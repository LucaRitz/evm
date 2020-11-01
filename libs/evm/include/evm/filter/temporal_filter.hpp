#pragma once

#include "../data.hpp"
#include <vector>
#include <opencv2/opencv.hpp>
#include "../macro_definition.hpp"

using std::vector;
using cv::Mat;

namespace evm {
    class EXPORT TemporalFilter {
    public:
        virtual TemporalFiltered operator()(const SpatialFiltered& spatialFiltered, double fps) = 0;
    };
}