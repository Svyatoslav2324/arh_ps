#pragma once

#include <ostream>

class Simulation;

class StepView {
public:
    void render(
        const Simulation& simulation,
        std::ostream& out
    ) const;
};