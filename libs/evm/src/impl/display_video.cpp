#include "../../include/evm/impl/display_video.hpp"

evm::DisplayVideo::DisplayVideo(RoiReconstructor& roiReconstructor, int framesPerSec, const string& name, int width, int height) :
    Display(roiReconstructor, framesPerSec),
    _writer(name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(width, height),true) {

}

evm::DisplayVideo::~DisplayVideo() {
    _writer.release();
}

void evm::DisplayVideo::display(const Mat& frame, int framesPerSec) {
    _writer.write(frame);
}