#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include "laplace_pyramid.hpp"

using std::queue;
using std::vector;
using std::mutex;
using std::thread;

namespace proc {

    struct Value {
        vector<LaplacePyramid> _pyramids;
        double _lowerFreq;
        double _upperFreq;
        double _samplingRate;
    };

    class QueueWorker {
    public:
        QueueWorker();

        void add(Value value);

    private:
        thread _thread;
        queue<Value> _queue;
        mutex _mutex;

        void work(queue<Value>& queue);
    };

}