#include "../../include/evm/impl/webcam_capture.hpp"
#include <opencv2/opencv.hpp>

evm::WebcamCapture::WebcamCapture() : _videoCapture() {
    _videoCapture.open(0);
    // TODO: May handle error!
}

evm::WebcamCapture::~WebcamCapture() {
    _videoCapture.release();
}

Mat evm::WebcamCapture::frame() {
    Mat frame;
    _videoCapture >> frame;

    return frame;
}

int evm::WebcamCapture::fps() {
    return -1;
}