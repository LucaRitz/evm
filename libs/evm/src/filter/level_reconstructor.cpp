#include "evm/filter/level_reconstructor.hpp"

evm::LevelReconstructor::LevelReconstructor(int level) : _level(level) {

}

vector<Mat> evm::LevelReconstructor::operator()(vector<Mat> originals, const vector<shared_ptr<Pyramid>>& amplified) {
    vector<Mat> results;

    for (int i = 0; i < originals.size(); i++) {
        Mat original = originals.at(i);
        Mat lowest_level = amplified.at(i)->at(_level);
        cv::resize(lowest_level, lowest_level, original.size(), 0, 0, cv::INTER_LINEAR);
        results.emplace_back(original + lowest_level);
    }

    return results;
}