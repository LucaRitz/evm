#pragma once

#include "temporal_filter.hpp"
#include "../data.hpp"
#include "../macro_definition.hpp"

namespace evm {
    class EXPORT IdealBandpassTemporalFilter : public TemporalFilter {
    public:
        IdealBandpassTemporalFilter(double lowerFreq, double upperFreq, int sampleRate);

        TemporalFiltered operator()(const SpatialFiltered& spatialFiltered);

    private:
        double _lowerFreq;
        double _upperFreq;
        int _sampleRate;

        static Mat concat(int level, const vector<shared_ptr<Pyramid>>& pyramids);
        static Mat buildFilterOfFrequencyBand(Mat& filter, double lowerBound, double upperBound, int sampleRate);
        static void insert(const Mat& value, int originalHeight, vector<shared_ptr<Pyramid>>& vec);
    };
}