#include "../include/processing/queue_worker.hpp"

proc::QueueWorker::QueueWorker() : _thread(&QueueWorker::work, this, std::ref(_queue)) {

}

void proc::QueueWorker::add(Value value) {
    _mutex.lock();
    _queue.emplace(value);
    _mutex.unlock();
}

void proc::QueueWorker::QueueWorker::work(queue<Value> &queue) {
    // TODO: Create filter and so on.

}