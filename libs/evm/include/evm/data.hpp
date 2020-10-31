#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <future>
#include "pyramid.hpp"
#include "macro_definition.hpp"
#include <memory>

using cv::Mat;
using std::vector;
using std::promise;
using std::shared_ptr;

namespace evm {
    struct EXPORT Position {
        int _x;
        int _y;
    };

    struct EXPORT Roi {
        Mat _roi;
        Position _position;
    };

    struct EXPORT OutputData {
        vector<Mat> _originals;
        vector<Roi> _processed_rois;
    };

    struct EXPORT InputData {
        vector<Mat> _originals;
        vector<Roi> _rois;
        int _fps;
        promise<OutputData> _promise;
    };

    struct EXPORT SpatialFiltered {
        vector<shared_ptr<Pyramid>> _spatialFiltered;
        int _levels;
    };

    struct EXPORT TemporalFiltered {
        vector<shared_ptr<Pyramid>> _temporalFiltered;
    };
}