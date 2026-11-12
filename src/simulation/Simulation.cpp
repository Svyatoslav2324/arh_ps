#include "Simulation.hpp"

#include <stdexcept>
#include <utility>

Simulation::Simulation(
    SimulationConfig config,
    std::uint64_t seed
)
    : config_(std::move(config)),
      seed_(seed),
      buffer_(static_cast<std::size_t>(config_.bufferCapacity)),
      rng_(seed_),
      placementDispatcher_(
          buffer_,
          statistics_,
          requests_
      ),
      selectionDispatcher_(
          workers_,
          buffer_,
          calendar_,
          requests_,
          rng_
      )
{
    if (!config_.validate()) {
        throw std::invalid_argument("Invalid simulation configuration");
    }
}

void Simulation::initialize() {
    currentTime_ = 0.0;
    generatedCount_ = 0;
    nextRequestId_ = 0;

    calendar_.clear();
    buffer_.clear();
    statistics_.reset();

    sources_.clear();
    workers_.clear();
    requests_.clear();

    lastEvent_.reset();

    rng_.seed(seed_);

    sources_.reserve(config_.sourceCount);
    workers_.reserve(config_.workerCount);

    for (int i = 0; i < config_.sourceCount; ++i) {
        sources_.emplace_back(
            i + 1,
            config_.arrivalRates[i]
        );
    }

    for (int i = 0; i < config_.workerCount; ++i) {
        workers_.emplace_back(
            i + 1,
            config_.serviceMin,
            config_.serviceMax
        );
    }

    for (auto& source : sources_) {
        double interval = source.nextInterval(rng_);

        calendar_.schedule(
            Event(
                currentTime_ + interval,
                EventKind::ARRIVAL,
                source.getId()
            )
        );
    }
}

bool Simulation::step() {
    if (calendar_.isEmpty()) {
        return false;
    }

    Event event = calendar_.popNext();
    lastEvent_ = event;
    currentTime_ = event.time;

    switch (event.kind) {
        case EventKind::ARRIVAL:
            handleArrival(event);
            break;

        case EventKind::RELEASE:
            handleRelease(event);
            break;
    }

    return true;
}

void Simulation::handleArrival(const Event& event) {
    int sourceIndex = event.entityId - 1;

    MonitoringSource& source = sources_.at(sourceIndex);

    int requestId = ++nextRequestId_;

    auto [it, inserted] = requests_.emplace(
        requestId,
        source.createRequest(currentTime_)
    );

    if (!inserted) {
        throw std::logic_error("Duplicate request ID");
    }

    TelemetryRequest& request = it->second;

    statistics_.onGenerated(request);

    ++generatedCount_;

    if (generatedCount_ < config_.requestLimit) {
        double interval = source.nextInterval(rng_);

        calendar_.schedule(
            Event(
                currentTime_ + interval,
                EventKind::ARRIVAL,
                source.getId()
            )
        );
    } else {
        calendar_.removeArrivals();
    }

    bool started = selectionDispatcher_.startIncoming(
        requestId,
        currentTime_
    );

    if (!started) {
        placementDispatcher_.enqueueOrReplace(requestId);
    }
}

void Simulation::handleRelease(const Event& event) {
    int workerIndex = event.entityId - 1;

    Worker& worker = workers_.at(workerIndex);

    int requestId = worker.finish(currentTime_);

    TelemetryRequest& request = requests_.at(requestId);

    request.finishService(currentTime_);

    statistics_.onServed(request);

    while (selectionDispatcher_.startFromQueue(currentTime_)) {
    }
}

SimulationResult Simulation::run() {
    while (step()) {
    }

    return statistics_.buildResult(
        currentTime_,
        workers_
    );
}

double Simulation::getCurrentTime() const {
    return currentTime_;
}

int Simulation::getGeneratedCount() const {
    return generatedCount_;
}

const EventCalendar& Simulation::getCalendar() const {
    return calendar_;
}

const QueueBuffer& Simulation::getBuffer() const {
    return buffer_;
}

const std::vector<Worker>& Simulation::getWorkers() const {
    return workers_;
}

const std::unordered_map<int, TelemetryRequest>&
Simulation::getRequests() const {
    return requests_;
}

const std::optional<Event>&
Simulation::getLastEvent() const {
    return lastEvent_;
}