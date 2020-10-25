#pragma once

#include "spatial_filter.hpp"
#include "../macro_definition.hpp"

namespace evm {
    class EXPORT LaplaceSpatialFilter : public SpatialFilter {
    public:
        explicit LaplaceSpatialFilter(int levels); // TODO: Ignore levels

        SpatialFiltered operator()(const vector<Mat>& frames);

    private:
        int _levels;
    };
}