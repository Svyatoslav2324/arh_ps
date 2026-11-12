#include "MonitoringSource.hpp"

MonitoringSource::MonitoringSource(int id, double lambda):
    id_(id),
    lambda_(lambda),
    generatedCount_(0),
    distribution_(lambda) 
{}

double MonitoringSource::nextInterval(std::mt19937_64& rng) {
    return distribution_(rng);
}

TelemetryRequest MonitoringSource::createRequest(double now) {
    generatedCount_++;
    return TelemetryRequest(id_, generatedCount_, now);
}

int MonitoringSource::getId() const {
    return id_;
}
    
int MonitoringSource::getGeneratedCount() const {
    return generatedCount_;
}