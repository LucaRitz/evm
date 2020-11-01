#include "../include/evm/fps_calculator.hpp"

evm::FpsCalculator::FpsCalculator(int refreshAfter):
    _refreshAfter(refreshAfter),
    _sampleNumber(0),
    _currentFps(30) {

}

void evm::FpsCalculator::startMeasurement() {
    _start = std::chrono::high_resolution_clock::now();
}

void evm::FpsCalculator::stopMeasurement() {
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = stop - _start;

    _fps.push(1/elapsed.count());

    if((_fps.size() == _refreshAfter) && (_sampleNumber == _refreshAfter))
    {
        // Empty queue and store sum
        double fpsSum = 0;
        while(!_fps.empty()) {
            fpsSum += _fps.front();
            _fps.pop();
        }

        // Calculate average FPS
        _currentFps = fpsSum / _refreshAfter;
        _sampleNumber = 0;
    }
}

void evm::FpsCalculator::incSample() {
    _sampleNumber++;
}

double evm::FpsCalculator::fps() {
    return _currentFps;
}