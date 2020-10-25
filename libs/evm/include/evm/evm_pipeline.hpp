#pragma once

#include "macro_definition.hpp"
#include "data.hpp"
#include <thread>
#include <future>
#include <atomic>
#include <queue>
#include <mutex>

using std::thread;
using std::future;
using std::atomic;
using std::queue;
using std::mutex;
using evm::OutputData;

namespace evm {

    class SpatialFilter;
    class TemporalFilter;
    class Amplifier;
    class Reconstructor;

    class EXPORT EvmPipeline {
    public:
        EvmPipeline(SpatialFilter* spatialFilter, TemporalFilter* temporalFilter, Amplifier* amplifier,
                    Reconstructor* reconstructor);

        future<OutputData> calculate(InputData&& input);
        void stop();


    private:
        atomic<bool> _running;
        queue<InputData> _queue;
        mutex _mutex;
        SpatialFilter* _spatialFilter;
        TemporalFilter* _temporalFilter;
        Amplifier* _amplifier;
        Reconstructor* _reconstructor;
        thread _thread;

        void work(atomic<bool>& running, queue<InputData>& queue, mutex& mut, SpatialFilter& spatialFilter,
                TemporalFilter& temporalFilter, Amplifier& amplifier, Reconstructor& reconstructor);
    };
}