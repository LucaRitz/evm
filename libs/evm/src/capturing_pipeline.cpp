#include "../include/evm/capturing_pipeline.hpp"
#include <opencv2/opencv.hpp>

evm::CapturingPipeline::CapturingPipeline(Capture& capture, RoiCapture& roiCapture, Processor& processor) :
    _running(true),
    _capture(&capture),
    _roiCapture(&roiCapture),
    _processor(&processor),
    _thread(&CapturingPipeline::work, this, std::ref(_running), std::ref(*_capture), std::ref(*_roiCapture), std::ref(*_processor)) {

}

void evm::CapturingPipeline::stop(bool doJoin) {
    _running = false;
    _processor->stop();
    if (doJoin) {
        join();
    }
}

void evm::CapturingPipeline::join() {
    _thread.join();
}

void evm::CapturingPipeline::work(atomic<bool>& running, Capture& capture, RoiCapture& roiCapture, Processor& processor) {
    while (running) {
        auto frame = capture.frame();
        if (frame.empty()) {
            stop(false);
        } else {
            auto roiFrame = roiCapture.roi(frame);
            if (!roiFrame._roi.empty()) {
                processor.process(frame, roiFrame);
            }
        }
    }
}