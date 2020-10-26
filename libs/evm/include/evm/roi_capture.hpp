#pragma once

#include "macro_definition.hpp"
#include "data.hpp"
#include "roi_reconstructor.hpp"
#include <opencv2/opencv.hpp>

using cv::Mat;

namespace evm {

    class EXPORT RoiCapture {
    public:
        virtual ~RoiCapture() = default;
        virtual Roi roi(const Mat& frame) = 0;
        virtual RoiReconstructor& getReconstructor() = 0;
    };
}