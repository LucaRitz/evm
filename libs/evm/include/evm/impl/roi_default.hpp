#pragma once

#include "../roi_capture.hpp"
#include "roi_default_reconstructor.hpp"
#include "../macro_definition.hpp"

namespace evm {
    class EXPORT RoiDefault: public RoiCapture {
    public:
        Roi roi(const Mat& frame);
        virtual RoiReconstructor& getReconstructor();

    private:
        RoiDefaultReconstructor _roiReconstructor;
    };
}