#include "../include/evm/display.hpp"
#include "../include/evm/fps_calculator.hpp"

evm::Display::Display(RoiReconstructor& roiReconstructor) :
        _running(true),
        _finishIfDone(false),
        _stopped(false),

        _queue(),
        _mutex(),
        _roiReconstructor(roiReconstructor),
        _thread(&Display::work, this, std::ref(_running), std::ref(_finishIfDone), std::ref(_stopped), std::ref(_queue),
                std::ref(_mutex)) {
}

void evm::Display::show(future<OutputData>& frames) {
    _mutex.lock();
    _queue.push(std::move(frames));
    _mutex.unlock();
}

void evm::Display::display(const Mat &frame, double framesPerSec, double calculationFps) {
    cv::imshow("Output", frame);
    double calcDelay = calculationFps > 0 ? 1000/calculationFps : 0;
    int delay = std::max(1.0, std::floor(1000/framesPerSec - calcDelay));
    cv::waitKey(delay);
    //cv::waitKey(0);
}

void
evm::Display::work(atomic<bool>& running, atomic<bool>& finishIfDone, atomic<bool>& stopped,
                   queue<future<evm::OutputData>>& queue, mutex& mut) {
    FpsCalculator fpsCalculator{32, 0};
    while (running) {
        fpsCalculator.startMeasurement();
        mut.lock();
        if (!queue.empty()) {
            auto data = std::move(queue.front());
            queue.pop();
            std::cout << "QUEUE: " << queue.size() << std::endl;
            mut.unlock();
            data.wait();
            auto output = data.get();
            for (int i = 0; i < output._processed_rois.size(); i++) {
                Mat converted;
                auto frame = _roiReconstructor.reconstruct(output._originals.at(i), output._processed_rois.at(i));

                double min, max;
                minMaxLoc(frame, &min, &max);
                frame.convertTo(converted, CV_8UC3, 255.0 / (max - min), -min * 255.0 / (max - min));

                fpsCalculator.incSample();
                fpsCalculator.stopMeasurement();
                display(converted, output._fps, fpsCalculator.fps() + 0.1);
                fpsCalculator.startMeasurement();
            }
            continue;
        } else if (finishIfDone) {
            running = false;
        }
        mut.unlock();
    }
    stopped = true;
}

void evm::Display::stop(bool waitUntilDone) {
    if (!waitUntilDone) {
        _running = false;
    } else {
        _finishIfDone = true;
    }
}

void evm::Display::join() {
    _thread.join();
}

bool evm::Display::stopped() {
    return _stopped;
}