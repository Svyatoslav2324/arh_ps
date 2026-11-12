#include "SelectionDispatcher.hpp"

#include <stdexcept>

SelectionDispatcher::SelectionDispatcher(
    std::vector<Worker>& workers,
    QueueBuffer& buffer,
    EventCalendar& calendar,
    std::unordered_map<int, TelemetryRequest>& requests,
    std::mt19937_64& rng
)
    : workers_(workers),
      buffer_(buffer),
      calendar_(calendar),
      requests_(requests),
      rng_(rng)
{
}

Worker* SelectionDispatcher::firstFreeWorker() {
    Worker* result = nullptr;

    for (auto& worker : workers_) {
        if (!worker.isFree()) {
            continue;
        }

        if (result == nullptr || worker.getId() < result->getId()) {
            result = &worker;
        }
    }

    return result;
}

void SelectionDispatcher::startService(
    Worker& worker,
    int requestId,
    double now
) {
    TelemetryRequest& request = requests_.at(requestId);

    if (request.getState() != RequestState::CREATED &&
        request.getState() != RequestState::QUEUED) {
        throw std::logic_error("Invalid request state");
    }

    double finishTime = worker.start(requestId, now, rng_);

    request.startService(now);

    calendar_.schedule(
        Event(finishTime, EventKind::RELEASE, worker.getId())
    );
}

bool SelectionDispatcher::startIncoming(
    int requestId,
    double now
) {
    Worker* worker = firstFreeWorker();

    if (worker == nullptr) {
        return false;
    }

    TelemetryRequest& request = requests_.at(requestId);

    if (request.getState() != RequestState::CREATED) {
        throw std::logic_error("Request must be CREATED");
    }

    startService(*worker, requestId, now);

    return true;
}

bool SelectionDispatcher::startFromQueue(double now) {
    if (buffer_.isEmpty()) {
        return false;
    }

    Worker* worker = firstFreeWorker();

    if (worker == nullptr) {
        return false;
    }

    int requestId = buffer_.removeNewest();

    TelemetryRequest& request = requests_.at(requestId);

    if (request.getState() != RequestState::QUEUED) {
        throw std::logic_error("Request must be QUEUED");
    }

    startService(*worker, requestId, now);

    return true;
}