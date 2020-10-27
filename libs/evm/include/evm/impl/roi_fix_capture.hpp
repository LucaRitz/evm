#pragma once

#include "../roi_capture.hpp"
#include "../macro_definition.hpp"
#include "roi_position_reconstructor.hpp"

namespace evm {

    class EXPORT RoiFixCapture : public RoiCapture {
    public:
        RoiFixCapture(int x, int y, int width, int height);
        virtual Roi roi(const Mat& frame) override;
        virtual RoiReconstructor& getReconstructor() override;

    private:
        RoiPositionReconstructor _roiReconstructor;
        cv::Rect _rect;
    };
}