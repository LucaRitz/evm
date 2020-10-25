#pragma once

#include "../roi_capture.hpp"
#include "../macro_definition.hpp"

namespace cv {
    class Mat;
}

using cv::Mat;

namespace evm {
    class EXPORT RoiDefault: public RoiCapture {
    public:
        Mat roi(const Mat& frame);
    };
}