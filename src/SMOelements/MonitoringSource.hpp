#pragma once
#include "TelemetryRequest.hpp"
#include <random>

class MonitoringSource {
public:
    MonitoringSource(int id, double lambda);
    double nextInterval(std::mt19937_64& rng);
    TelemetryRequest createRequest(double now);
    int getId() const;
    int getGeneratedCount() const;
private:
    int id_;
    double lambda_;
    int generatedCount_;
    std::exponential_distribution<double> distribution_;
};