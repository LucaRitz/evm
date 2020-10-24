#pragma once

#include "macro_definition.hpp"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <utility>

namespace cv {
    class Mat;
}

using cv::Mat;
using std::unordered_map;
using std::vector;
using std::pair;
using std::hash;
using Dimension = pair<int, int>;

namespace proc {
    struct hash_pair {
        template <class T1, class T2>
        size_t operator()(const pair<T1, T2>& p) const
        {
            auto hash1 = hash<T1>{}(p.first);
            auto hash2 = hash<T2>{}(p.second);
            return hash1 ^ hash2;
        }
    };
}

using Filters = unordered_map<Dimension, Mat, proc::hash_pair>;

namespace proc {

    class EXPORT Filter {
    public:
        Filter(double lowerBound, double upperBound, int sampleRate);

        Mat operator*(Mat toFilter);

    private:
        Filters _filter;
        double _lowerBound;
        double _upperBound;
        int _sampleRate;

        Mat findAndCreateFilterIfNotExist(const Mat& toFilter);

        static vector<Mat> splitByChannel(const Mat& src);
        static Mat mergeChannels(const vector<Mat>& mats);
        static Mat buildFilterOfFrequencyBand(Mat& filter, double lowerBound, double upperBound, int sampleRate);
    };
}