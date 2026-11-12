#include "PlacementDispatcher.hpp"

#include <stdexcept>

PlacementDispatcher::PlacementDispatcher(
    QueueBuffer& buffer,
    StatisticsCollector& statistics,
    std::unordered_map<int, TelemetryRequest>& requests
)
    : buffer_(buffer),
      statistics_(statistics),
      requests_(requests)
{
}

void PlacementDispatcher::enqueueOrReplace(int requestId) {
    TelemetryRequest& request = requests_.at(requestId);

    if (request.getState() != RequestState::CREATED) {
        throw std::logic_error("Request must be CREATED");
    }

    if (buffer_.isFull()) {
        if (buffer_.isEmpty()) {
            request.reject();
            statistics_.onRejected(request);
            return;
        }

        int oldestId = buffer_.removeOldest();

        TelemetryRequest& oldestRequest = requests_.at(oldestId);

        oldestRequest.reject();
        statistics_.onRejected(oldestRequest);
    }

    buffer_.append(requestId);
    request.makeQueued();
}