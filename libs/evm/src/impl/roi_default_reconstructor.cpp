#include "../../include/evm/impl/roi_default_reconstructor.hpp"

Mat evm::RoiDefaultReconstructor::reconstruct(const Mat& original, const Roi& roi) {
    return roi._roi;
}