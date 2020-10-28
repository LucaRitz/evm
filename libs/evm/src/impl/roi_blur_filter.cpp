#include "../../include/evm/impl/roi_blur_filter.hpp"

evm::RoiBlurFilter::RoiBlurFilter(int width, int height): _maskSize(width, height) {

}

evm::Roi evm::RoiBlurFilter::filter(const Roi& image) {
    Mat filtered;
    cv::blur(image._roi, filtered, _maskSize);
    return Roi{filtered, image._position};
}