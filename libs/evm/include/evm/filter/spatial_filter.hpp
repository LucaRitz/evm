#pragma once

#include "../data.hpp"
#include "../macro_definition.hpp"
#include <vector>

using std::vector;

namespace evm {
    class EXPORT SpatialFilter {
    public:
        virtual SpatialFiltered operator()(const vector<Roi>& frames) = 0;
    };
}