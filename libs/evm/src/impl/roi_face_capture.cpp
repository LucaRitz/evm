#include "../../include/evm/impl/roi_face_capture.hpp"

#include <opencv2/opencv.hpp>

evm::Roi evm::RoiFaceCapture::roi(const Mat& frame) {
    return Roi{frame, Position{0, 0}};
}

evm::RoiReconstructor& evm::RoiFaceCapture::getReconstructor() {
    return _roiReconstructor;
}