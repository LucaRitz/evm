#include "../../include/evm/filter/reconstructor.hpp"

vector<Mat> evm::Reconstructor::operator()(vector<Mat> originals, const vector<shared_ptr<Pyramid>>& amplified) {
    vector<Mat> results;

    for (int i = 0; i < originals.size(); i++) {
        results.emplace_back(originals.at(i) + amplified.at(i)->reconstruct());
    }

    return results;
}