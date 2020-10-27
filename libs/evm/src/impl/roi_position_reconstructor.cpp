#include "../../include/evm/impl/roi_position_reconstructor.hpp"

Mat evm::RoiPositionReconstructor::reconstruct(const Mat& original, const Roi& roi) {
    Mat roiMat = roi._roi;
    double min,max;
    minMaxLoc(original, &min, &max);
    //cv::normalize(roiMat, roiMat, min, max, cv::NORM_MINMAX);

    roiMat.copyTo(original(cv::Rect(roi._position._x, roi._position._y, roiMat.cols, roiMat.rows)));
    //cv::cvtColor(roiMat, roiMat, cv::COLOR_BGR2GRAY);
    return roiMat;
}