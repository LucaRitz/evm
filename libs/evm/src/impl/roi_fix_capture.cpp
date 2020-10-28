#include "../../include/evm/impl/roi_fix_capture.hpp"

evm::RoiFixCapture::RoiFixCapture(int x, int y, int width, int height):
_rect(x, y, width, height) {
}

evm::Roi evm::RoiFixCapture::roi(const Mat& frame) {
    return Roi{frame(_rect), Position{_rect.x, _rect.y}};
}

evm::RoiReconstructor& evm::RoiFixCapture::getReconstructor() {
    return _roiReconstructor;
}