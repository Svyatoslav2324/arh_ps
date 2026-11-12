#pragma once

#include "SimulationConfig.hpp"
#include "EventCalendar.hpp"
#include "MonitoringSource.hpp"
#include "Worker.hpp"
#include "QueueBuffer.hpp"
#include "StatisticsCollector.hpp"
#include "PlacementDispatcher.hpp"
#include "SelectionDispatcher.hpp"

#include <cstdint>
#include <random>
#include <unordered_map>
#include <vector>
#include <optional>

class Simulation {
public:
    explicit Simulation(
        SimulationConfig config,
        std::uint64_t seed = 42
    );

    void initialize();
    bool step();
    SimulationResult run();

    double getCurrentTime() const;
    int getGeneratedCount() const;

    const EventCalendar& getCalendar() const;
    const QueueBuffer& getBuffer() const;
    const std::vector<Worker>& getWorkers() const;

    const std::unordered_map<int, TelemetryRequest>&
    getRequests() const;

    const std::optional<Event>& getLastEvent() const;
private:
    void handleArrival(const Event& event);
    void handleRelease(const Event& event);

    SimulationConfig config_;

    std::uint64_t seed_;

    double currentTime_ = 0.0;
    int generatedCount_ = 0;
    int nextRequestId_ = 0;

    EventCalendar calendar_;
    QueueBuffer buffer_;

    std::vector<MonitoringSource> sources_;
    std::vector<Worker> workers_;

    std::unordered_map<int, TelemetryRequest> requests_;

    StatisticsCollector statistics_;

    std::mt19937_64 rng_;

    PlacementDispatcher placementDispatcher_;
    SelectionDispatcher selectionDispatcher_;

    std::optional<Event> lastEvent_;
};