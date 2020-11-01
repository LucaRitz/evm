#pragma once

#include "macro_definition.hpp"
#include <opencv2/opencv.hpp>

namespace cv {
    class Mat;
}

using cv::Mat;
using cv::Size;

namespace evm {
    class EXPORT Capture {
    public:
        virtual ~Capture() = default;
        virtual Mat frame() = 0;
        virtual int fps() = 0;
        virtual Size size() = 0;
    };
}