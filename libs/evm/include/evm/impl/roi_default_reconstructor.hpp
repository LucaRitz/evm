#pragma once

#include "../roi_reconstructor.hpp"
#include "../macro_definition.hpp"

namespace evm {

    class EXPORT RoiDefaultReconstructor : public RoiReconstructor {
    public:
        virtual Mat reconstruct(const Mat& original, const Roi& roi) override;
    };
}