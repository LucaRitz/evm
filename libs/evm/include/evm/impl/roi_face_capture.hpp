#pragma once

#include "../roi_capture.hpp"
#include "roi_position_reconstructor.hpp"
#include "../macro_definition.hpp"
#include <opencv2/face.hpp>

namespace evm {
    class EXPORT RoiFaceCapture: public RoiCapture {
    public:
        RoiFaceCapture();
        virtual Roi roi(const Mat& frame) override;
        virtual RoiReconstructor& getReconstructor() override;

    private:
        RoiPositionReconstructor _roiReconstructor;
        cv::CascadeClassifier _faceDetector;
        cv::Ptr<cv::face::Facemark> _facemark;
    };
}