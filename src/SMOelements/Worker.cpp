#include "Worker.hpp"
#include <stdexcept>

Worker::Worker(int id, double serviceMin, double serviceMax):
    id_(id),
    currentRequestId_(std::nullopt),
    serviceStartTime_(std::nullopt),
    totalBusyTime_(0.0),
    distribution_(serviceMin, serviceMax) 
{}

bool Worker::isFree() const {
    if (currentRequestId_) {
        return false;
    }

    return true;
}

double Worker::start(int requestId, double now, std::mt19937_64& rng) {
    if (currentRequestId_) {
        throw std::runtime_error("worker is busy");
    }

    currentRequestId_ = requestId;
    serviceStartTime_ = now;
    auto servingTime = distribution_(rng);

    return now + servingTime;
}

int Worker::finish(double now) {
    if (!currentRequestId_) {
        throw std::runtime_error("worker is free already");
    }

    totalBusyTime_ += now - *serviceStartTime_;
    auto requestId = *currentRequestId_;
    currentRequestId_ = std::nullopt;
    serviceStartTime_ = std::nullopt;

    return requestId;
}

int Worker::getId() const {
    return id_;
}

std::optional<int> Worker::getCurrentRequestId() const {
    return currentRequestId_;
}

double Worker::getTotalBusyTime() const {
    return totalBusyTime_;
}