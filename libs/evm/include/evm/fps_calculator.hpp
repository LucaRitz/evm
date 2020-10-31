#pragma once

#include <queue>
#include <chrono>

using std::queue;

namespace evm {

    class FpsCalculator {
    public:
        FpsCalculator(int refreshAfter);

        void startMeasurement();
        void stopMeasurement();
        void incSample();
        int fps();

    private:
        const int _refreshAfter;
        int _sampleNumber;
        int _currentFps;
        queue<int> _fps;
        std::chrono::time_point<std::chrono::steady_clock> _start;
    };
}