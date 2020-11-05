#pragma once

#include "../display.hpp"
#include <string>

using std::string;

namespace evm {
    class EXPORT DisplayVideo : public Display {
    public:
        DisplayVideo(RoiReconstructor& roiReconstructor, int framesPerSec, const string& name, int width, int height);
        ~DisplayVideo();

        virtual void display(const Mat& frame, double framesPerSec, double calculationFps) override;

    private:
        cv::VideoWriter _writer;
    };
}