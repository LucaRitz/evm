#pragma once

#include "reconstructor.hpp"
#include "../macro_definition.hpp"

namespace evm {
    class EXPORT LevelReconstructor : public Reconstructor {
    public:
        explicit LevelReconstructor(int level);

        virtual vector<Roi> operator()(const vector<Roi>& originals, const vector<shared_ptr<Pyramid>>& amplified) const override;

    private:
        int _level;
    };
}