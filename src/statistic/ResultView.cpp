#include "ResultView.hpp"

#include <iomanip>
#include <string>

void ResultView::render(
    const SimulationResult& result,
    std::ostream& out
) const {
    auto oldFlags = out.flags();
    auto oldPrecision = out.precision();

    out << std::fixed << std::setprecision(4);

    out << "\n========== SIMULATION RESULTS ==========\n";

    out << "Simulation time: "
        << result.totalTime << '\n';

    out << "Total generated: "
        << result.totalGenerated << '\n';

    out << "Total served: "
        << result.totalServed << '\n';

    out << "Total rejected: "
        << result.totalRejected << '\n';

    out << "Rejection probability: "
        << result.rejectionProbability << '\n';

    out << "\n========== SOURCE STATISTICS ==========\n";

    out << std::left
        << std::setw(10) << "Source"
        << std::setw(12) << "Generated"
        << std::setw(12) << "Served"
        << std::setw(12) << "Rejected"
        << std::setw(12) << "P(reject)"
        << '\n';

    out << std::string(58, '-') << '\n';

    for (const auto& stat : result.sourceStatistics) {
        out << std::left
            << std::setw(10) << stat.sourceId
            << std::setw(12) << stat.generated
            << std::setw(12) << stat.served
            << std::setw(12) << stat.rejected
            << std::setw(12) << stat.rejectionProbability
            << '\n';
    }

    out << "\n========== TIME STATISTICS ==========\n";

    out << std::left
        << std::setw(10) << "Source"
        << std::setw(14) << "Mean wait"
        << std::setw(14) << "Mean service"
        << std::setw(14) << "Mean sojourn"
        << '\n';

    out << std::string(52, '-') << '\n';

    for (const auto& stat : result.sourceStatistics) {
        out << std::left << std::setw(10) << stat.sourceId;

        if (stat.served == 0) {
            out << std::setw(14) << "-"
                << std::setw(14) << "-"
                << std::setw(14) << "-";
        } else {
            out << std::setw(14) << stat.meanWait
                << std::setw(14) << stat.meanService
                << std::setw(14) << stat.meanSojourn;
        }

        out << '\n';
    }

    out << "\n========== VARIANCES ==========\n";

    out << std::left
        << std::setw(10) << "Source"
        << std::setw(18) << "Wait variance"
        << std::setw(18) << "Service variance"
        << '\n';

    out << std::string(46, '-') << '\n';

    for (const auto& stat : result.sourceStatistics) {
        out << std::left << std::setw(10) << stat.sourceId;

        if (stat.served == 0) {
            out << std::setw(18) << "-"
                << std::setw(18) << "-";
        } else {
            out << std::setw(18) << stat.varianceWait
                << std::setw(18) << stat.varianceService;
        }

        out << '\n';
    }

    out << "\n========== WORKER STATISTICS ==========\n";

    out << std::left
        << std::setw(12) << "Worker"
        << std::setw(18) << "Utilization"
        << '\n';

    out << std::string(30, '-') << '\n';

    for (const auto& stat : result.workerStatistics) {
        out << std::left
            << std::setw(12) << stat.workerId
            << std::setw(18) << stat.utilization
            << '\n';
    }

    out << "\n========================================\n";

    out.flags(oldFlags);
    out.precision(oldPrecision);
}