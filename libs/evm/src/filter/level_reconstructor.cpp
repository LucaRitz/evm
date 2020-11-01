#include "evm/filter/level_reconstructor.hpp"
#include <algorithm>

evm::LevelReconstructor::LevelReconstructor(int level) : _level(level) {

}

vector<evm::Roi> evm::LevelReconstructor::operator()(const vector<Roi>& originals, const vector<shared_ptr<Pyramid>>& amplified) const {
    vector<Roi> results;

    for (int i = 0; i < originals.size(); i++) {
        Mat original = originals.at(i)._roi;
        Mat levelMat = amplified.at(i)->at(_level);
        if (!levelMat.empty()) {
            cv::resize(levelMat, levelMat, original.size(), 0, 0, cv::INTER_LINEAR);
            /*Mat channels[3];
            cv::split(levelMat, channels);

            double min, max;
            minMaxLoc(channels[2], &min, &max);
            std::cout << "min " << min << " max " << max << std::endl;
            cv::threshold(channels[2], channels[2], 0.95f * max, max, cv::THRESH_TRUNC);
            //cv::threshold(channels[2], channels[2], 0.95f * max, max, cv::THRESH_BINARY);

            channels[0] = cv::Scalar(0);
            channels[1] = cv::Scalar(0);

            cv::merge(channels, 3, levelMat);*/

            //results.emplace_back(Roi{original + lowest_level, originals.at(i)._position});
            results.emplace_back(Roi{original + levelMat, originals.at(i)._position});
        } else {
            //results.emplace_back(Roi{original, originals.at(i)._position});
        }
    }

    return results;
}