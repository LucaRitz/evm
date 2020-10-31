#pragma once

#include "macro_definition.hpp"
#include "data.hpp"
#include <thread>
#include <future>
#include <atomic>
#include <queue>
#include <mutex>
#include <functional>

using std::thread;
using std::future;
using std::atomic;
using std::queue;
using std::mutex;
using evm::OutputData;
using std::function;

namespace evm {

    class SpatialFilter;
    class TemporalFilter;
    class Amplifier;
    class Reconstructor;

    class EXPORT EvmPipeline {
    public:
        EvmPipeline(SpatialFilter& spatialFilter, TemporalFilter& temporalFilter, Amplifier& amplifier,
                    Reconstructor& reconstructor);

        future<OutputData> calculate(InputData&& input);
        void stop(bool waitUntilDone = false);
        void join();
        bool stopped();
        void setStoppedListener(function<void(bool)> listener);

    private:
        atomic<bool> _running;
        atomic<bool> _finishIfDone;
        atomic<bool> _stopped;
        function<void(bool)> _stoppedListener;
        queue<InputData> _queue;
        mutex _mutex;
        SpatialFilter& _spatialFilter;
        TemporalFilter& _temporalFilter;
        Amplifier& _amplifier;
        Reconstructor& _reconstructor;
        thread _thread;

        vector<Mat> _originals;
        vector<Roi> _rois;

        void work(atomic<bool>& running, atomic<bool>& finishIfDone, atomic<bool>& stopped, queue<InputData>& queue,
                  mutex& mut, SpatialFilter& spatialFilter, TemporalFilter& temporalFilter, Amplifier& amplifier,
                  Reconstructor& reconstructor);
    };
}