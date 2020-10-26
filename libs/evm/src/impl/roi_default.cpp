#include "../../include/evm/impl/roi_default.hpp"

#include <opencv2/opencv.hpp>

evm::Roi evm::RoiDefault::roi(const Mat& frame) {
    return Roi{frame, Position{0, 0}};
}

evm::RoiReconstructor& evm::RoiDefault::getReconstructor() {
    return _roiReconstructor;
}