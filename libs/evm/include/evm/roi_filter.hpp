#pragma once

#include "macro_definition.hpp"
#include "data.hpp"

namespace evm {

    class EXPORT RoiFilter {
    public:
        virtual Roi filter(const Roi& image) = 0;
    };
}