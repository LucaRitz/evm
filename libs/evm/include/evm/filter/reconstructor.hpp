#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include "../macro_definition.hpp"
#include "../data.hpp"

using cv::Mat;
using std::vector;

namespace evm {

    class EXPORT Reconstructor {
    public:
        vector<Mat> operator()(vector<Mat> originals, const vector<shared_ptr<Pyramid>>& amplified);
    };
}