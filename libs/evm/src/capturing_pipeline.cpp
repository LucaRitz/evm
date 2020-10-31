#include "../include/evm/capturing_pipeline.hpp"
#include <opencv2/opencv.hpp>

evm::CapturingPipeline::CapturingPipeline(Capture& capture, RoiCapture& roiCapture, Processor& processor,
                                          RoiFilter* roiFilter) :
        _running(true),
        _fpsCalculator(32),
        _capture(&capture),
        _roiCapture(&roiCapture),
        _processor(&processor),
        _roiFilter(roiFilter),
        _thread(&CapturingPipeline::work, this, std::ref(_running), std::ref(*_capture), std::ref(*_roiCapture),
                std::ref(_roiFilter), std::ref(*_processor)) {

}

void evm::CapturingPipeline::stop() {
    _running = false;
    join();
}

void evm::CapturingPipeline::join() {
    if (_thread.joinable()) {
        _thread.join();
    }
}

void evm::CapturingPipeline::work(atomic<bool>& running, Capture& capture, RoiCapture& roiCapture,
                                  RoiFilter*& roiFilter, Processor& processor) {
    while (running) {
        _fpsCalculator.startMeasurement();
        auto frame = capture.frame();
        if (frame.empty()) {
            _running = false;
            processor.stop(true);
            processor.join();
        } else {
            _fpsCalculator.incSample();
            frame.convertTo(frame, CV_32FC3);
            auto roiFrame = roiCapture.roi(frame);
            if (roiFilter != nullptr) {
                roiFrame = roiFilter->filter(roiFrame);
            }
            if (!roiFrame._roi.empty()) {
                //_fpsCalculator.stopMeasurement();
                int fps = capture.fps();
                if (capture.fps() == -1) {
                    _fpsCalculator.stopMeasurement();
                    fps = _fpsCalculator.fps();
                }
                processor.process(frame, roiFrame, fps);
            }
        }
    }

    if (!processor.stopped()) {
        processor.stop();
        processor.join();
    }
}