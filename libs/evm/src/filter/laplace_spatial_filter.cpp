#include "../../include/evm/filter/laplace_spatial_filter.hpp"
#include "laplace_pyramid.hpp"

evm::LaplaceSpatialFilter::LaplaceSpatialFilter(int levels) : _levels(levels) {

}

evm::SpatialFiltered evm::LaplaceSpatialFilter::operator()(const vector<Roi>& frames) {
    vector<shared_ptr<Pyramid>> outputData;

    for (const auto& frame : frames) {
        outputData.push_back(std::make_shared<LaplacePyramid>(frame._roi, _levels));
    }

    return SpatialFiltered{outputData, _levels};
}