#pragma once

#include "macro_definition.hpp"

namespace cv {
    class Mat;
}

using cv::Mat;

namespace evm {
    class EXPORT RoiCapture {
    public:
        virtual ~RoiCapture() = default;
        virtual Mat roi(const Mat& frame) = 0;
    };
}