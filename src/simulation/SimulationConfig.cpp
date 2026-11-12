#include "SimulationConfig.hpp"
#include <utility>

SimulationConfig::SimulationConfig(
    int sourceCount,
    int workerCount,
    int bufferCapacity,
    std::vector<double>&& arrivalRates,
    double serviceMin,
    double serviceMax,
    int requestLimit
)
    : sourceCount(sourceCount),
      workerCount(workerCount),
      bufferCapacity(bufferCapacity),
      arrivalRates(std::move(arrivalRates)),
      serviceMin(serviceMin),
      serviceMax(serviceMax),
      requestLimit(requestLimit) 
{}

bool SimulationConfig::validate() {
    if (sourceCount < 1) {
        return false;
    }
    if (workerCount < 1) {
        return false;
    }
    if (bufferCapacity < 1) {
        return false;
    }
    if (arrivalRates.size() != sourceCount) {
        return false;
    }
    if (serviceMin <= 0) {
        return false;
    }
    if (serviceMax < serviceMin) {
        return false;
    }
    if (requestLimit < 1) {
        return false;
    }

    return true;
}


