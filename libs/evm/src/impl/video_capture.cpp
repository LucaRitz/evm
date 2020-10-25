#include "../../include/evm/impl/video_capture.hpp"

evm::VideoCapture::VideoCapture(string fileName) : _videoCapture(fileName) {
    // TODO: May handle error
}

evm::VideoCapture::~VideoCapture() {
    _videoCapture.release();
}

Mat evm::VideoCapture::frame() {
    Mat frame;
    _videoCapture >> frame;
    frame.convertTo(frame, CV_32FC3);
    return frame;
}