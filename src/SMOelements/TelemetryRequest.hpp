#pragma once
#include <optional>

enum class RequestState {
    CREATED,
    QUEUED,
    SERVING,
    SERVED,
    REJECTED
};

class TelemetryRequest {
public:
    TelemetryRequest(int, int, double);
    void makeQueued();
    void startService(double now);
    void finishService(double now);
    void reject();
    std::optional<double> getWaitingTime() const;
    std::optional<double> getServiceTime() const;
    int getSourceId() const { return sourceId_; }
    int getSequenceNumber() const { return sequenceNumber_; }
    double getArrivalTime() const { return arrivalTime_; }
    RequestState getState() const { return state_; }
private:
    int sourceId_;
    int sequenceNumber_;
    double arrivalTime_;
    std::optional<double> startTime_;
    std::optional<double> finishTime_;
    RequestState state_;
};