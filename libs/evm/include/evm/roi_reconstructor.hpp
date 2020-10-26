#pragma once

#include "macro_definition.hpp"
#include "data.hpp"

namespace evm {

    class EXPORT RoiReconstructor {
    public:
        virtual Mat reconstruct(const Mat& original, const Roi& roi) = 0;
    };
}