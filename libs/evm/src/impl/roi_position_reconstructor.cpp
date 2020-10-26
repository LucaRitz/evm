#include "../../include/evm/impl/roi_position_reconstructor.hpp"

Mat evm::RoiPositionReconstructor::reconstruct(const Mat& original, const Roi& roi) {
    roi._roi.copyTo(original(cv::Rect(roi._position._x, roi._position._y, roi._roi.cols, roi._roi.rows)));
    return original;
}