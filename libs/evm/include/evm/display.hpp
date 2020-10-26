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
        Display(RoiReconstructor& roiReconstructor, int framesPerSec);

        virtual void show(future<OutputData>& frames);
        void stop();

    private:
        atomic<bool> _running;
        queue<future<OutputData>> _queue;
        mutex _mutex;
        RoiReconstructor& _roiReconstructor;
        thread _thread;

        void work(atomic<bool>& running, queue<future<OutputData>>& queue, mutex& mut, int framesPerSec);
    };
}