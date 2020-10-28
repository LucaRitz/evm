#pragma once

#include "../roi_filter.hpp"

namespace evm {
    class EXPORT RoiBlurFilter : public RoiFilter {
    public:
        RoiBlurFilter(int width, int height);
        virtual Roi filter(const Roi& image) override;

    private:
        cv::Size _maskSize;
    };
}