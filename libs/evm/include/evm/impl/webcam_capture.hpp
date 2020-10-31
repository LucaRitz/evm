#pragma once

#include "../capture.hpp"
#include "../macro_definition.hpp"
#include <opencv2/opencv.hpp>

using cv::Mat;
using cv::VideoCapture;

namespace evm {
    class EXPORT WebcamCapture : public Capture {
    public:
        WebcamCapture();
        ~WebcamCapture();

        Mat frame();
        virtual int fps() override;

    private:
        VideoCapture _videoCapture;
    };
}