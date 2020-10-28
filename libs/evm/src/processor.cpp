#include "../include/evm/processor.hpp"
#include <opencv2/opencv.hpp>

evm::Processor::Processor(EvmPipeline& evmPipeline, Display& display, int fps) :
    _evmPipeline(&evmPipeline),
    _display(&display),
    _bufferSize(calcBufferSize(fps)) {
    _evmPipeline->setStoppedListener([this](bool waitUntilDone) { this->evmPipelineStopped(waitUntilDone); });
}

void evm::Processor::process(Mat& original, Roi& roi) {
    _originals.push_back(original);
    _rois.push_back(roi);
    if (_originals.size() == _bufferSize) {
        pushToPipeline();
    }
}

void evm::Processor::stop(bool waitUntilDone) {
    pushToPipeline();
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

void evm::Processor::pushToPipeline() {
    if (!_originals.empty()) {
        future<OutputData> result = _evmPipeline->calculate(InputData{_originals, _rois});
        _display->show(result);
        _originals.clear();
        _rois.clear();
    }
}

int evm::Processor::calcBufferSize(int fps) {
    // Calculate number of images needed to represent 2 seconds of film material
    unsigned int round = (unsigned int) std::max(2*fps,16);
    round--;
    round |= round >> 1;
    round |= round >> 2;
    round |= round >> 4;
    round |= round >> 8;
    round |= round >> 16;
    round++;

    return round;
}