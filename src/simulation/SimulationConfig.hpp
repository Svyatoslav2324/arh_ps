#pragma once
#include <vector>

struct SimulationConfig
{
    int sourceCount;
    int workerCount;
    int bufferCapacity;
    std::vector<double> arrivalRates;
    double serviceMin;
    double serviceMax;
    int requestLimit;

    SimulationConfig(int sourceCount,
                    int workerCount,
                    int bufferCapacity,
                    std::vector<double>&& arrivalRates,
                    double serviceMin,
                    double serviceMax,
                    int requestLimit);

    bool validate();
};
