#include "StatisticsCollector.hpp"
#include "Worker.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>


void StatisticsCollector::onGenerated(
    const TelemetryRequest& request
) {
    if (request.getState() != RequestState::CREATED) {
        throw std::logic_error("Request must be CREATED");
    }

    auto& stats = sources_[request.getSourceId()];

    ++stats.generated;
}


void StatisticsCollector::onRejected(
    const TelemetryRequest& request
) {
    if (request.getState() != RequestState::REJECTED) {
        throw std::logic_error("Request must be REJECTED");
    }

    auto& stats = sources_.at(request.getSourceId());

    if (stats.served + stats.rejected >= stats.generated) {
        throw std::logic_error("Invalid rejection statistics");
    }

    ++stats.rejected;
}


void StatisticsCollector::onServed(
    const TelemetryRequest& request
) {
    if (request.getState() != RequestState::SERVED) {
        throw std::logic_error("Request must be SERVED");
    }

    auto waitingTime = request.getWaitingTime();
    auto serviceTime = request.getServiceTime();

    if (!waitingTime || !serviceTime) {
        throw std::logic_error("Request has incomplete timing data");
    }

    if (!std::isfinite(*waitingTime) ||
        !std::isfinite(*serviceTime) ||
        *waitingTime < 0.0 ||
        *serviceTime < 0.0) {
        throw std::logic_error("Invalid request timing data");
    }

    auto& stats = sources_.at(request.getSourceId());

    if (stats.served + stats.rejected >= stats.generated) {
        throw std::logic_error("Invalid service statistics");
    }

    ++stats.served;

    stats.sumWait += *waitingTime;
    stats.sumWaitSquared += *waitingTime * *waitingTime;

    stats.sumService += *serviceTime;
    stats.sumServiceSquared += *serviceTime * *serviceTime;

    stats.sumSojourn += *waitingTime + *serviceTime;
}


SimulationResult StatisticsCollector::buildResult(
    double totalTime,
    const std::vector<Worker>& workers
) const {
    if (!std::isfinite(totalTime) || totalTime < 0.0) {
        throw std::invalid_argument("Invalid simulation duration");
    }

    SimulationResult result;
    result.totalTime = totalTime;

    for (const auto& [sourceId, counters] : sources_) {
        SourceStat stat;

        stat.sourceId = sourceId;

        stat.generated = counters.generated;
        stat.served = counters.served;
        stat.rejected = counters.rejected;

        if (counters.generated > 0) {
            stat.rejectionProbability =
                static_cast<double>(counters.rejected) /
                counters.generated;
        }

        if (counters.served > 0) {
            const double n =
                static_cast<double>(counters.served);

            stat.meanWait = counters.sumWait / n;
            stat.meanService = counters.sumService / n;
            stat.meanSojourn = counters.sumSojourn / n;

            stat.varianceWait = std::max(
                0.0,
                counters.sumWaitSquared / n -
                stat.meanWait * stat.meanWait
            );

            stat.varianceService = std::max(
                0.0,
                counters.sumServiceSquared / n -
                stat.meanService * stat.meanService
            );
        }

        result.totalGenerated += counters.generated;
        result.totalServed += counters.served;
        result.totalRejected += counters.rejected;

        result.sourceStatistics.push_back(stat);
    }

    if (result.totalGenerated > 0) {
        result.rejectionProbability =
            static_cast<double>(result.totalRejected) /
            result.totalGenerated;
    }

    for (const auto& worker : workers) {
        WorkerStat stat;

        stat.workerId = worker.getId();

        if (totalTime > 0.0) {
            stat.utilization =
                worker.getTotalBusyTime() / totalTime;
        }

        result.workerStatistics.push_back(stat);
    }

    return result;
}

void StatisticsCollector::reset() {
    sources_.clear();
}