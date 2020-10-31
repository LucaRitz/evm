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
    return frame;
}

int evm::VideoCapture::fps() {
    return _videoCapture.get(cv::CAP_PROP_FPS);

}