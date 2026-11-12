#pragma once

#include "QueueBuffer.hpp"
#include "StatisticsCollector.hpp"
#include "TelemetryRequest.hpp"

#include <unordered_map>

class PlacementDispatcher {
public:
    PlacementDispatcher(
        QueueBuffer& buffer,
        StatisticsCollector& statistics,
        std::unordered_map<int, TelemetryRequest>& requests
    );

    void enqueueOrReplace(int requestId);

private:
    QueueBuffer& buffer_;
    StatisticsCollector& statistics_;
    std::unordered_map<int, TelemetryRequest>& requests_;
};