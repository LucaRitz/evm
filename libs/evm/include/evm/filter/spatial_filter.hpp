#pragma once

#include "../data.hpp"
#include "../macro_definition.hpp"
#include <vector>
#include <opencv2/opencv.hpp>

using std::vector;
using cv::Mat;

namespace evm {
    class EXPORT SpatialFilter {
    public:
        virtual SpatialFiltered operator()(const vector<Mat>& frames) = 0;
    };
}