#pragma once

#include "../macro_definition.hpp"
#include <vector>
#include "../data.hpp"

using std::vector;

namespace evm {

    class EXPORT Amplifier {
    public:
        Amplifier(vector<int> factors);

        vector<shared_ptr<Pyramid>> operator*(const vector<shared_ptr<Pyramid>>& pyramids);

    private:
        vector<int> _factors;
    };
}