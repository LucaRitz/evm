#include "../include/evm/processor.hpp"
#include <opencv2/opencv.hpp>

evm::Processor::Processor(EvmPipeline& evmPipeline, Display& display, int bufferSize) :
    _evmPipeline(&evmPipeline),
    _display(&display),
    _bufferSize(bufferSize) {

}

void evm::Processor::process(Mat& original, Roi& roi) {
    _originals.push_back(original);
    _rois.push_back(roi);

    if (_originals.size() == _bufferSize) {
        future<OutputData> result = _evmPipeline->calculate(InputData{_originals, _rois});
        _display->show(result);
        _originals.clear();
        _rois.clear();
    }
}

void evm::Processor::stop() {
    _evmPipeline->stop();
    _display->stop();
}