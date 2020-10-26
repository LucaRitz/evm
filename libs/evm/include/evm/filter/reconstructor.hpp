#pragma once

#include <vector>
#include "../macro_definition.hpp"
#include "../data.hpp"

using std::vector;

namespace evm {

    class EXPORT Reconstructor {
    public:
        virtual vector<Roi> operator()(const vector<Roi>& originals, const vector<shared_ptr<Pyramid>>& amplified) const;
    };
}