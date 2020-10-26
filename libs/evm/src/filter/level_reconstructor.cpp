#include "evm/filter/level_reconstructor.hpp"

evm::LevelReconstructor::LevelReconstructor(int level) : _level(level) {

}

vector<evm::Roi> evm::LevelReconstructor::operator()(const vector<Roi>& originals, const vector<shared_ptr<Pyramid>>& amplified) const {
    vector<Roi> results;

    for (int i = 0; i < originals.size(); i++) {
        Mat original = originals.at(i)._roi;
        Mat lowest_level = amplified.at(i)->at(_level);
        cv::resize(lowest_level, lowest_level, original.size(), 0, 0, cv::INTER_LINEAR);
        results.emplace_back(Roi{original + lowest_level, originals.at(i)._position});
    }

    return results;
}