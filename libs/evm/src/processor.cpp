#include "../include/evm/processor.hpp"
#include <opencv2/opencv.hpp>

evm::Processor::Processor(EvmPipeline& evmPipeline, Display& display) :
    _evmPipeline(&evmPipeline),
    _display(&display) {
    _evmPipeline->setStoppedListener([this](bool waitUntilDone) { this->evmPipelineStopped(waitUntilDone); });
}

void evm::Processor::process(Mat& original, Roi& roi, double fps) {
    future<OutputData> result = _evmPipeline->calculate(InputData{vector<Mat>{original}, vector<Roi>{roi}, fps});
    _display->show(result);
}

void evm::Processor::stop(bool waitUntilDone) {
    _evmPipeline->stop(waitUntilDone);
}

void evm::Processor::join() {
    _evmPipeline->join();
    _display->join();
}

bool evm::Processor::stopped() {
    return _evmPipeline->stopped() && _display->stopped();
}

void evm::Processor::evmPipelineStopped(bool waitUntilDone) {
    if (!_display->stopped()) {
        _display->stop(waitUntilDone);
    }
}