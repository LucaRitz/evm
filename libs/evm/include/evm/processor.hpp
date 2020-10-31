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
        Processor(EvmPipeline& evmPipeline, Display& display);

        void process(Mat& original, Roi& roi, int fps);
        void stop(bool waitUntilDone = false);
        void join();
        bool stopped();

    private:
        EvmPipeline* _evmPipeline;
        Display* _display;

        void evmPipelineStopped(bool waitUntilDone);
    };
}