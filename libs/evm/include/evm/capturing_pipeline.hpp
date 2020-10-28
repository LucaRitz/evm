#pragma once

#include "capture.hpp"
#include "roi_capture.hpp"
#include "processor.hpp"
#include "roi_filter.hpp"

#include "macro_definition.hpp"
#include <atomic>
#include <thread>

using std::atomic;
using std::thread;

namespace evm {

    class EXPORT CapturingPipeline {
    public:
        CapturingPipeline(Capture& capture, RoiCapture& roiCapture, Processor& processor,
                          RoiFilter* roiFilter = nullptr);

        void stop();
        void join();

    private:
        atomic<bool> _running;
        Capture* _capture;
        RoiCapture* _roiCapture;
        RoiFilter* _roiFilter;
        Processor* _processor;
        thread _thread;

        void work(atomic<bool>& running, Capture& capture, RoiCapture& roiCapture, RoiFilter*& roiFilter,
                  Processor& processor);
    };
}