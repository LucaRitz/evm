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
        double fps();

    private:
        const int _refreshAfter;
        int _sampleNumber;
        double _currentFps;
        queue<int> _fps;
        std::chrono::time_point<std::chrono::steady_clock> _start;
    };
}