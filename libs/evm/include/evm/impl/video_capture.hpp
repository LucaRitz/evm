#pragma once

#include "../capture.hpp"
#include "../macro_definition.hpp"
#include <opencv2/opencv.hpp>
#include <string>

using cv::Mat;
using std::string;

namespace evm {
    class EXPORT VideoCapture : public Capture {
    public:
        explicit VideoCapture(string fileName);
        ~VideoCapture();

        virtual Mat frame() override;
        virtual int fps() override;
        virtual Size size() override;

    private:
        cv::VideoCapture _videoCapture;
    };
}