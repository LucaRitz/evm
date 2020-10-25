#pragma once

#include "../include/evm/pyramid.hpp"

namespace evm {
    class LaplacePyramid : public Pyramid {
    public:
        LaplacePyramid(const Mat& image, int levels);
    };
}