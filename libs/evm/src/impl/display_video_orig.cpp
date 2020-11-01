#include "../../include/evm/impl/display_video_orig.hpp"
#include <algorithm>

evm::DisplayVideoOrig::DisplayVideoOrig(RoiReconstructor& roiReconstructor, int framesPerSec, const string& name, int width, int height, const string& orig) :
    Display(roiReconstructor, framesPerSec),
    _capture(orig),
    _writer(name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(width + static_cast<int>(_capture.get(
            cv::CAP_PROP_FRAME_WIDTH)), std::max(height, static_cast<int>(_capture.get(cv::CAP_PROP_FRAME_HEIGHT)))), true) {

}

evm::DisplayVideoOrig::~DisplayVideoOrig() {
    _writer.release();
    _capture.release();
}

void evm::DisplayVideoOrig::display(const Mat& frame, int framesPerSec) {

    Mat origFrame;
    _capture >> origFrame;

    if (!origFrame.empty()) {
        Mat newFrame{cv::Size(origFrame.cols + frame.cols, std::max(origFrame.rows, frame.rows)), CV_8UC3};
        origFrame.copyTo(newFrame(cv::Rect(0, 0, origFrame.cols, origFrame.rows)));


       /* double min, max;
        minMaxLoc(origFrame, &min, &max);
        cv::normalize(frame, frame, max, min, cv::NORM_MINMAX);*/

        frame.copyTo(newFrame(cv::Rect(origFrame.cols, 0, frame.cols, frame.rows)));
        _writer.write(newFrame);
    } else {
        std::cout << "NO FRAME" << std::endl;
    }
}