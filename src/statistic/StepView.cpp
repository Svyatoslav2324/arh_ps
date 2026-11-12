#include "StepView.hpp"
#include "Simulation.hpp"

#include <iomanip>

namespace {

const char* eventKindToString(EventKind kind) {
    switch (kind) {
        case EventKind::ARRIVAL:
            return "ARRIVAL";

        case EventKind::RELEASE:
            return "RELEASE";
    }

    return "UNKNOWN";
}

}

void StepView::render(
    const Simulation& simulation,
    std::ostream& out
) const {
    auto oldFlags = out.flags();
    auto oldPrecision = out.precision();

    out << std::fixed << std::setprecision(3);

    out << "\n========== SIMULATION STEP ==========\n";

    out << "Model time: "
        << simulation.getCurrentTime() << '\n';

    if (simulation.getLastEvent()) {
        const Event& event = *simulation.getLastEvent();

        out << "Last event: "
            << eventKindToString(event.kind)
            << " | Entity: "
            << event.entityId
            << '\n';
    }    

    out << "Generated: "
        << simulation.getGeneratedCount() << '\n';

    const auto& requests = simulation.getRequests();

    std::size_t served = 0;
    std::size_t rejected = 0;

    for (const auto& [id, request] : requests) {
        if (request.getState() == RequestState::SERVED) {
            ++served;
        } else if (request.getState() == RequestState::REJECTED) {
            ++rejected;
        }
    }

    out << "Served: " << served << '\n';
    out << "Rejected: " << rejected << '\n';

    out << "\nBuffer:\n";

    QueueBuffer buffer = simulation.getBuffer();

    if (buffer.isEmpty()) {
        out << "Empty\n";
    }

    while (!buffer.isEmpty()) {
        int requestId = buffer.removeOldest();

        const auto& request = requests.at(requestId);

        out << "Request "
            << request.getSourceId()
            << "."
            << request.getSequenceNumber()
            << " (ID: "
            << requestId
            << ")\n";
    }

    out << "\nWorkers:\n";

    for (const auto& worker : simulation.getWorkers()) {
        out << "Worker " << worker.getId() << ": ";

        if (worker.isFree()) {
            out << "FREE\n";
        } else {
            int requestId = *worker.getCurrentRequestId();

            const auto& request = requests.at(requestId);

            out << "BUSY, request "
                << request.getSourceId()
                << "."
                << request.getSequenceNumber()
                << '\n';
        }
    }

    out << "\nEvent calendar:\n";

    EventCalendar calendar = simulation.getCalendar();

    if (calendar.isEmpty()) {
        out << "Empty\n";
    }

    while (!calendar.isEmpty()) {
        Event event = calendar.popNext();

        out << "t = " << event.time
            << " | " << eventKindToString(event.kind)
            << " | Entity: " << event.entityId
            << '\n';
    }

    out << "=====================================\n";

    out.flags(oldFlags);
    out.precision(oldPrecision);
}