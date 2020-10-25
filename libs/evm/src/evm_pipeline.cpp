#include "../include/evm/evm_pipeline.hpp"

#include "../include/evm/filter/spatial_filter.hpp"
#include "../include/evm/filter/temporal_filter.hpp"
#include "../include/evm/filter/amplifier.hpp"
#include "../include/evm/filter/reconstructor.hpp"

using std::promise;

evm::EvmPipeline::EvmPipeline(SpatialFilter* spatialFilter, TemporalFilter* temporalFilter, Amplifier* amplifier,
                              Reconstructor* reconstructor) :
    _running(true),
    _queue(),
    _mutex(),
    _spatialFilter(spatialFilter),
    _temporalFilter(temporalFilter),
    _amplifier(amplifier),
    _reconstructor(reconstructor),
    _thread(&EvmPipeline::work, this, std::ref(_running), std::ref(_queue), std::ref(_mutex), std::ref(*spatialFilter),
            std::ref(*temporalFilter),std::ref(*amplifier), std::ref(*reconstructor)) {

}

future<OutputData> evm::EvmPipeline::calculate(InputData&& input) {
    future<OutputData> future = input._promise.get_future();
    _mutex.lock();
    _queue.push(std::move(input));
    _mutex.unlock();
    return std::move(future);
}

void evm::EvmPipeline::stop() {
    _running = false;
    _thread.join();
}

void evm::EvmPipeline::work(atomic<bool>& running, queue<InputData>& queue, mutex& mut, SpatialFilter& spatialFilter,
          TemporalFilter& temporalFilter, Amplifier& amplifier, Reconstructor& reconstructor) {
    while(running) {
        mut.lock();
        if (!queue.empty()) {
            auto input = std::move(queue.front());
            queue.pop();
            mut.unlock();

            auto spatialFiltered = spatialFilter(input._rois);
            auto temporalFiltered = temporalFilter(spatialFiltered);
            auto amplified = amplifier * temporalFiltered._temporalFiltered;
            auto reconstructed = reconstructor(input._rois, amplified);
            input._promise.set_value(OutputData{input._originals, reconstructed});
            continue;
        }
        mut.unlock();
    }
}