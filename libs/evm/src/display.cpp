#include "../include/evm/display.hpp"

evm::Display::Display(int framesPerSec) :
    _running(true),
    _queue(),
    _mutex(),
    _thread(&Display::work, this, std::ref(_running), std::ref(_queue), std::ref(_mutex), framesPerSec) {
}

void evm::Display::show(future<OutputData>& frames) {
    _mutex.lock();
    _queue.push(std::move(frames));
    _mutex.unlock();
}

void evm::Display::work(atomic<bool>& running, queue<future<evm::OutputData>>& queue, mutex& mut, int framesPerSec) {
    while(running) {
        mut.lock();
        if (!queue.empty()) {
            auto data = std::move(queue.front());
            queue.pop();
            mut.unlock();
            data.wait();
            auto frames = data.get()._processed_rois;
            for(auto& frame : frames) {
                Mat converted;
                double min,max;
                minMaxLoc(frame, &min, &max);
                frame.convertTo(converted, CV_8UC3, 255.0/(max-min), -min * 255.0/(max-min));
                cv::imshow("Output", converted);
                cv::waitKey(1000/framesPerSec);
            }
            continue;
        }
        mut.unlock();
    }
}

void evm::Display::stop() {
    _running = false;
    _thread.join();
}
