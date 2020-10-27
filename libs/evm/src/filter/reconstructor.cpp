#include "../../include/evm/filter/reconstructor.hpp"

vector<evm::Roi> evm::Reconstructor::operator()(const vector<Roi>& originals, const vector<shared_ptr<Pyramid>>& amplified) const {
    vector<Roi> results;

    for (int i = 0; i < originals.size(); i++) {
        Mat result = originals.at(i)._roi + amplified.at(i)->reconstruct();
        //Mat result = amplified.at(i)->reconstruct();
        results.emplace_back(Roi{result, originals.at(i)._position});
    }

    return results;
}