#include "../include/evm/display.hpp"

evm::Display::Display(RoiReconstructor& roiReconstructor, int framesPerSec) :
        _running(true),
        _finishIfDone(false),
        _stopped(false),

        _queue(),
        _mutex(),
        _roiReconstructor(roiReconstructor),
        _thread(&Display::work, this, std::ref(_running), std::ref(_finishIfDone), std::ref(_stopped), std::ref(_queue),
                std::ref(_mutex), framesPerSec) {
}

void evm::Display::show(future<OutputData>& frames) {
    _mutex.lock();
    _queue.push(std::move(frames));
    _mutex.unlock();
}

void evm::Display::display(const Mat &frame, int framesPerSec) {
    cv::imshow("Output 2", frame);
    cv::waitKey(1000/framesPerSec + 1);
    //cv::waitKey(0);
}

void
evm::Display::work(atomic<bool>& running, atomic<bool>& finishIfDone, atomic<bool>& stopped,
                   queue<future<evm::OutputData>>& queue, mutex& mut, int framesPerSec) {
    while (running) {
        mut.lock();
        if (!queue.empty()) {
            auto data = std::move(queue.front());
            queue.pop();
            mut.unlock();
            data.wait();
            auto output = data.get();
            for (int i = 0; i < output._processed_rois.size(); i++) {
                Mat converted;
                auto frame = _roiReconstructor.reconstruct(output._originals.at(i), output._processed_rois.at(i));

                double min, max;
                minMaxLoc(frame, &min, &max);
                frame.convertTo(converted, CV_8UC3, 255.0 / (max - min), -min * 255.0 / (max - min));
                display(converted, framesPerSec);
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