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

        virtual Mat frame() override;
        virtual int fps() override;
        virtual Size size() override;

    private:
        VideoCapture _videoCapture;
    };
}