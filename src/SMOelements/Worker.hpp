#pragma once
#include <optional>
#include <random>

class Worker {
public:
    Worker(int id, double serviceMin, double serviceMax);
    bool isFree() const;
    double start(int requestId, double now, std::mt19937_64& rng);
    int finish(double now);
    int getId() const;
    std::optional<int> getCurrentRequestId() const;
    double getTotalBusyTime() const;
private:
    int id_;
    std::optional<int> currentRequestId_;
    std::optional<double> serviceStartTime_;
    double totalBusyTime_;
    std::uniform_real_distribution<double> distribution_;
};