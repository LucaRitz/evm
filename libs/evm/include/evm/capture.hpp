#pragma once

#include "macro_definition.hpp"

namespace cv {
    class Mat;
}

using cv::Mat;

namespace evm {
    class EXPORT Capture {
    public:
        virtual ~Capture() = default;
        virtual Mat frame() = 0;
    };
}