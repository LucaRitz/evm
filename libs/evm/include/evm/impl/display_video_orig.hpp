#pragma once

#include "../display.hpp"
#include <string>

using std::string;

namespace evm {
    class EXPORT DisplayVideoOrig : public Display {
    public:
        DisplayVideoOrig(RoiReconstructor& roiReconstructor, int framesPerSec, const string& name, int width, int height, const string& original);
        ~DisplayVideoOrig();

        virtual void display(const Mat& frame, int framesPerSec) override;

    private:
        cv::VideoCapture _capture;
        cv::VideoWriter _writer;
    };
}