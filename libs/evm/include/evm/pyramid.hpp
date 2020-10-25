#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include "macro_definition.hpp"

using std::vector;
using cv::Mat;

namespace evm {
    class EXPORT Pyramid {
    public:
        Pyramid() = default;
        virtual ~Pyramid() = default;

        Mat at(int level) const;
        Mat reconstruct() const;
        Pyramid operator*(const vector<int>& factors) const;

        void push_back(Mat value);
        int levels() const;

    protected:
        vector<Mat> _levels;
    };
}