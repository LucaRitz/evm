#pragma once

#include "capture.hpp"
#include "roi_capture.hpp"
#include "processor.hpp"

#include "macro_definition.hpp"
#include <atomic>
#include <thread>

using std::atomic;
using std::thread;

namespace evm {

    class EXPORT CapturingPipeline {
    public:
        CapturingPipeline(Capture& capture, RoiCapture& roiCapture, Processor& processor);

        void stop();
        void join();

    private:
        atomic<bool> _running;
        Capture* _capture;
        RoiCapture* _roiCapture;
        Processor* _processor;
        thread _thread;

        void work(atomic<bool>& running, Capture& capture, RoiCapture& roiCapture, Processor& processor);
    };
}