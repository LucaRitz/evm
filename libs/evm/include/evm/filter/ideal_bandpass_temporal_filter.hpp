#pragma once

#include "temporal_filter.hpp"
#include "../data.hpp"
#include "../macro_definition.hpp"

#include <unordered_map>

using std::unordered_map;

namespace evm {
    class EXPORT IdealBandpassTemporalFilter : public TemporalFilter {
    public:
        IdealBandpassTemporalFilter(double lowerFreq, double upperFreq, int level = -1);

        TemporalFiltered operator()(const SpatialFiltered& spatialFiltered, int fps);

    private:
        double _lowerFreq;
        double _upperFreq;
        int _level;
        int _processedFrames;
        int _totalFrames;
        int _bufferSize;
        unordered_map<int, Mat> _filterMat;
        Pyramid _originalSizes;

        Mat concat(int level, Mat& dest, int maxSize, const vector<shared_ptr<Pyramid>>& pyramids);
        static int bufferSize(int fps);
        static void insert(const Mat& value, int originalHeight, vector<shared_ptr<Pyramid>>& vec);
        static Mat buildFilterOfFrequencyBand(Mat& filter, double lowerBound, double upperBound, int sampleRate);
    };
}