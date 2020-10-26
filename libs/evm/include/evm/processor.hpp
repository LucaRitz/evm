#pragma once

#include "macro_definition.hpp"
#include "evm_pipeline.hpp"
#include "display.hpp"
#include <vector>

namespace cv {
    class Mat;
}

using std::vector;
using cv::Mat;

namespace evm {
    class EXPORT Processor {
    public:
        Processor(EvmPipeline& evmPipeline, Display& display, int bufferSize);

        void process(Mat& original, Roi& roi);
        void stop();

    private:
        EvmPipeline* _evmPipeline;
        Display* _display;
        int _bufferSize;
        vector<Mat> _originals;
        vector<Roi> _rois;
    };
}