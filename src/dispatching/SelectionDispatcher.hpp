#pragma once

#include "Worker.hpp"
#include "QueueBuffer.hpp"
#include "EventCalendar.hpp"
#include "TelemetryRequest.hpp"

#include <vector>
#include <unordered_map>
#include <random>

class SelectionDispatcher {
public:
    SelectionDispatcher(
        std::vector<Worker>& workers,
        QueueBuffer& buffer,
        EventCalendar& calendar,
        std::unordered_map<int, TelemetryRequest>& requests,
        std::mt19937_64& rng
    );

    Worker* firstFreeWorker();

    bool startIncoming(int requestId, double now);

    bool startFromQueue(double now);

private:
    void startService(
        Worker& worker,
        int requestId,
        double now
    );

    std::vector<Worker>& workers_;
    QueueBuffer& buffer_;
    EventCalendar& calendar_;
    std::unordered_map<int, TelemetryRequest>& requests_;
    std::mt19937_64& rng_;
};