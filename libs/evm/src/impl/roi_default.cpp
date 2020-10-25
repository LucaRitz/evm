#include "../../include/evm/impl/roi_default.hpp"

#include <opencv2/opencv.hpp>

Mat evm::RoiDefault::roi(const Mat& frame) {
    return frame;
}