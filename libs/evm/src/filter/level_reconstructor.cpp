#include "evm/filter/level_reconstructor.hpp"

evm::LevelReconstructor::LevelReconstructor(int level) : _level(level) {

}

vector<evm::Roi> evm::LevelReconstructor::operator()(const vector<Roi>& originals, const vector<shared_ptr<Pyramid>>& amplified) const {
    vector<Roi> results;

    for (int i = 0; i < originals.size(); i++) {
        Mat original = originals.at(i)._roi;
        Mat levelMat = amplified.at(i)->at(_level);
        cv::resize(levelMat, levelMat, original.size(), 0, 0, cv::INTER_LINEAR);
        //results.emplace_back(Roi{original + lowest_level, originals.at(i)._position});
        results.emplace_back(Roi{levelMat, originals.at(i)._position});
    }

    return results;
}