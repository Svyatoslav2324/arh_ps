#include "TelemetryRequest.hpp"

TelemetryRequest::TelemetryRequest(int sourceId, int sequenceNumber, double arrivalTime):
    sourceId_(sourceId),
    sequenceNumber_(sequenceNumber),
    arrivalTime_(arrivalTime),
    startTime_(std::nullopt),
    finishTime_(std::nullopt),
    state_(RequestState::CREATED) 
{
}

void TelemetryRequest::makeQueued() {
    state_ = RequestState::QUEUED;
}

void TelemetryRequest::reject() {
    state_ = RequestState::REJECTED;
}

void TelemetryRequest::startService(double now) {
    startTime_ = now;
    state_ = RequestState::SERVING;
}

void TelemetryRequest::finishService(double now) {
    finishTime_ = now;
    state_ = RequestState::SERVED;
}

std::optional<double> TelemetryRequest::getWaitingTime() const {
    if (startTime_) {
        return *startTime_ - arrivalTime_;
    }

    return std::nullopt;
}

std::optional<double> TelemetryRequest::getServiceTime() const {
    if (startTime_ && finishTime_) {
        return *finishTime_ - *startTime_;
    }

    return std::nullopt;
}