#pragma once

#include "TelemetryRequest.hpp"

#include <cstddef>
#include <map>
#include <vector>

class Worker;

struct SourceStat {
    int sourceId = 0;

    std::size_t generated = 0;
    std::size_t served = 0;
    std::size_t rejected = 0;

    double rejectionProbability = 0.0;

    double meanWait = 0.0;
    double meanService = 0.0;
    double meanSojourn = 0.0;

    double varianceWait = 0.0;
    double varianceService = 0.0;
};


struct WorkerStat {
    int workerId = 0;
    double utilization = 0.0;
};


struct SimulationResult {
    std::size_t totalGenerated = 0;
    std::size_t totalServed = 0;
    std::size_t totalRejected = 0;

    double rejectionProbability = 0.0;
    double totalTime = 0.0;

    std::vector<SourceStat> sourceStatistics;
    std::vector<WorkerStat> workerStatistics;
};

class StatisticsCollector {
public:
    void onGenerated(const TelemetryRequest& request);
    void onRejected(const TelemetryRequest& request);
    void onServed(const TelemetryRequest& request);

    SimulationResult buildResult(
        double totalTime,
        const std::vector<Worker>& workers
    ) const;

    void reset();

private:

    struct SourceCounters {
        std::size_t generated = 0;
        std::size_t served = 0;
        std::size_t rejected = 0;

        double sumWait = 0.0;
        double sumWaitSquared = 0.0;

        double sumService = 0.0;
        double sumServiceSquared = 0.0;

        double sumSojourn = 0.0;
    };


    std::map<int, SourceCounters> sources_;
};