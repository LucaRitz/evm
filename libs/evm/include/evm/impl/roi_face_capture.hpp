#pragma once

#include "../roi_capture.hpp"
#include "roi_position_reconstructor.hpp"
#include "../macro_definition.hpp"

namespace evm {
    class EXPORT RoiFaceCapture: public RoiCapture {
    public:
        virtual Roi roi(const Mat& frame) override;
        virtual RoiReconstructor& getReconstructor() override;

    private:
        RoiPositionReconstructor _roiReconstructor;
    };
}