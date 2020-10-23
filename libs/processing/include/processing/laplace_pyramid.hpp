#pragma once

#include <vector>
#include "macro_definition.hpp"

namespace cv {
    class Mat;
}

using t_level = unsigned short;
using std::vector;
using cv::Mat;

namespace proc {
    class Filter;

    class EXPORT LaplacePyramid {
    public:
        explicit LaplacePyramid(cv::Mat& image, t_level levels=6);
        LaplacePyramid(const LaplacePyramid& other) = default;

        Mat reconstruct();
        Mat at(t_level level);
        t_level levels();

        LaplacePyramid norm(const LaplacePyramid& pyramid);
        LaplacePyramid operator+(const LaplacePyramid& other);
        LaplacePyramid operator*(const vector<double>& factor);
        LaplacePyramid operator*(Filter& filter);

    private:
        LaplacePyramid() = default;
        vector<cv::Mat> _pyramid;
    };
}