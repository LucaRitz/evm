#include "../include/evm/evm_pipeline.hpp"

#include "../include/evm/filter/spatial_filter.hpp"
#include "../include/evm/filter/temporal_filter.hpp"
#include "../include/evm/filter/amplifier.hpp"
#include "../include/evm/filter/reconstructor.hpp"

using std::promise;

evm::EvmPipeline::EvmPipeline(SpatialFilter& spatialFilter, TemporalFilter& temporalFilter, Amplifier& amplifier,
                              Reconstructor& reconstructor) :
    _running(true),
    _finishIfDone(false),
    _stopped(false),
    _stoppedListener(),
    _queue(),
    _mutex(),
    _spatialFilter(spatialFilter),
    _temporalFilter(temporalFilter),
    _amplifier(amplifier),
    _reconstructor(reconstructor),
    _thread(&EvmPipeline::work, this, std::ref(_running), std::ref(_finishIfDone), std::ref(_stopped), std::ref(_queue),
            std::ref(_mutex), std::ref(_spatialFilter), std::ref(_temporalFilter), std::ref(_amplifier),
            std::ref(_reconstructor)) {

}

future<OutputData> evm::EvmPipeline::calculate(InputData&& input) {
    future<OutputData> future = input._promise.get_future();
    _mutex.lock();
    _queue.push(std::move(input));
    _mutex.unlock();
    return std::move(future);
}

void evm::EvmPipeline::stop(bool waitUntilDone) {
    if (!waitUntilDone) {
        _running = false;
    } else {
        _finishIfDone = true;
    }
}

void evm::EvmPipeline::join() {
    _thread.join();
}

bool evm::EvmPipeline::stopped() {
    return _stopped;
}

void evm::EvmPipeline::setStoppedListener(function<void (bool)> listener) {
    _stoppedListener = std::move(listener);
}

void evm::EvmPipeline::work(atomic<bool>& running, atomic<bool>& finishIfDone, atomic<bool>& stopped,
                            queue<InputData>& queue, mutex& mut, SpatialFilter& spatialFilter,
                            TemporalFilter& temporalFilter, Amplifier& amplifier, Reconstructor& reconstructor) {
    while (running) {
        mut.lock();
        if (!queue.empty()) {
            auto input = std::move(queue.front());
            queue.pop();
            mut.unlock();

            _originals.insert(_originals.end(), input._originals.begin(), input._originals.end());
            _rois.insert(_rois.end(), input._rois.begin(), input._rois.end());

            auto spatialFiltered = spatialFilter(input._rois);
            auto temporalFiltered = temporalFilter(spatialFiltered, input._fps);

            if (!temporalFiltered._temporalFiltered.empty()) {
                int results = temporalFiltered._temporalFiltered.size();
                auto amplified = amplifier * temporalFiltered._temporalFiltered;

                vector<Roi> rois;
                rois.insert(rois.begin(), _rois.begin(), _rois.begin() + results);
                _rois.erase(_rois.begin(), _rois.begin() + results);
                auto reconstructed = reconstructor(rois, amplified);

                vector<Mat> originals;
                originals.insert(originals.begin(), _originals.begin(), _originals.begin() + results);
                _originals.erase(_originals.begin(), _originals.begin() + results);
                input._promise.set_value(OutputData{originals, reconstructed});
            } else {
                input._promise.set_value(OutputData{vector<Mat>{}, vector<Roi>{}});
            }

            continue;
        } else if (finishIfDone) {
            running = false;
        }
        mut.unlock();
    }
    stopped = true;
    _stoppedListener(finishIfDone);
}