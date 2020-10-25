#pragma once

#include "reconstructor.hpp"
#include "../macro_definition.hpp"

namespace evm {
    class EXPORT LevelReconstructor : public Reconstructor {
    public:
        LevelReconstructor(int level);

        vector<Mat> operator()(vector<Mat> originals, const vector<shared_ptr<Pyramid>>& amplified);

    private:
        int _level;
    };
}