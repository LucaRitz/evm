#include "../../include/evm/filter/amplifier.hpp"
#include "../../include/evm/pyramid.hpp"

evm::Amplifier::Amplifier(vector<int> factors) : _factors(std::move(factors)) {

}

vector<shared_ptr<evm::Pyramid>> evm::Amplifier::operator*(const vector<shared_ptr<Pyramid>>& pyramids) {
    vector<shared_ptr<Pyramid>> result;

    for(auto& pyramid : pyramids) {
        result.push_back(std::make_shared<Pyramid>((*pyramid) * _factors));
    }
    return result;
}
