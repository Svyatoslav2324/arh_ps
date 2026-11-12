#pragma once

#include "StatisticsCollector.hpp"

#include <ostream>

class ResultView {
public:
    void render(
        const SimulationResult& result,
        std::ostream& out
    ) const;
};