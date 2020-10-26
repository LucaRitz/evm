#include "../include/evm/pyramid.hpp"

Mat evm::Pyramid::at(int level) const {
    return _levels.at(level);
}

Mat evm::Pyramid::reconstruct() const {
    if (_levels.empty()) {
        return Mat{};
    }

    auto collapsed = _levels.at(0);

    for (int i = 1; i < _levels.size(); i++) {
        auto current = _levels.at(i);
        cv::pyrUp(collapsed, collapsed, current.size());

        cv::cvtColor(current, current, cv::COLOR_GRAY2RGB);
        collapsed += current;
    }

    return collapsed;
}

evm::Pyramid evm::Pyramid::operator*(const vector<int>& factors) const {
    Pyramid result;

    for(int i = 0; i < _levels.size(); i++) {
        Mat mat = _levels.at(i);
        if (factors.size() > i) {
            mat *= factors.at(i);
        }
        result.push_back(mat);
    }

    return result;
}

int evm::Pyramid::levels() const {
    return _levels.size();
}

void evm::Pyramid::push_back(Mat value) {
    _levels.push_back(value);
}