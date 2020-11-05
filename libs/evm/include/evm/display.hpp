#pragma once

#include "macro_definition.hpp"
#include "data.hpp"
#include "roi_reconstructor.hpp"

#include <future>
#include <vector>
#include <queue>
#include <atomic>
#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>

using std::future;
using std::vector;
using cv::Mat;
using std::queue;
using std::atomic;
using std::thread;
using std::mutex;

namespace evm {
    class EXPORT Display {
    public:
        explicit Display(RoiReconstructor& roiReconstructor);
        virtual ~Display() = default;

        void show(future<OutputData>& frames);
        virtual void display(const Mat& frame, double framesPerSec, double calculationFps);
        void stop(bool waitUntilDone = false);
        void join();
        bool stopped();

    private:
        atomic<bool> _running;
        atomic<bool> _finishIfDone;
        atomic<bool> _stopped;
        queue<future<OutputData>> _queue;
        mutex _mutex;
        RoiReconstructor& _roiReconstructor;
        thread _thread;

        void work(atomic<bool>& running, atomic<bool>& finishIfDone, atomic<bool>& stopped,
                  queue<future<OutputData>>& queue, mutex& mut);
    };
}