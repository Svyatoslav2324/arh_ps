#include "Simulation.hpp"
#include "StepView.hpp"
#include "ResultView.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <stdexcept>
#include <utility>

template<typename T>
T readValue(const std::string& prompt) {
    T value{};

    std::cout << prompt;

    if (!(std::cin >> value)) {
        throw std::runtime_error("Invalid input");
    }

    return value;
}

int main() {
    try {
        std::cout << "========== SMO SIMULATOR ==========\n";
        std::cout << "Variant 7: Server Monitoring\n\n";

        int sourceCount = readValue<int>("Source count: ");
        int workerCount = readValue<int>("Worker count: ");
        int bufferCapacity = readValue<int>("Buffer capacity: ");

        if (sourceCount <= 0 || sourceCount > 10000) {
            throw std::invalid_argument("Invalid source count");
        }

        std::vector<double> arrivalRates;
        arrivalRates.reserve(sourceCount);

        std::cout << "\nArrival rates:\n";

        for (int i = 0; i < sourceCount; ++i) {
            double lambda = readValue<double>(
                "Source " + std::to_string(i + 1) + ": "
            );

            arrivalRates.push_back(lambda);
        }

        std::cout << "\nService time distribution:\n";

        double serviceMin = readValue<double>("Minimum: ");
        double serviceMax = readValue<double>("Maximum: ");

        int requestLimit = readValue<int>("\nRequest limit (N): ");

        SimulationConfig config(
            sourceCount,
            workerCount,
            bufferCapacity,
            std::move(arrivalRates),
            serviceMin,
            serviceMax,
            requestLimit
        );

        if (!config.validate()) {
            throw std::invalid_argument(
                "Invalid simulation configuration"
            );
        }

        std::cout << "\n========== RUN MODE ==========\n";
        std::cout << "1. Step-by-step\n";
        std::cout << "2. Automatic\n";

        int mode = readValue<int>("\nSelect mode: ");

        if (mode != 1 && mode != 2) {
            throw std::invalid_argument("Invalid run mode");
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        Simulation simulation(config, 42);

        StepView stepView;
        ResultView resultView;

        simulation.initialize();

        if (mode == 1) {
            std::cout << "\nStep-by-step mode started.\n";

            while (simulation.step()) {
                stepView.render(simulation, std::cout);

                std::string command;

                while (true) {
                    std::cout
                        << "\nEnter - next step"
                        << "\na - automatic mode"
                        << "\nq - quit"
                        << "\n> ";

                    if (!std::getline(std::cin, command)) {
                        return 0;
                    }

                    if (command.empty() ||
                        command == "a" ||
                        command == "q") {
                        break;
                    }

                    std::cout << "Unknown command.\n";
                }

                if (command == "a") {
                    std::cout << "\nSwitching to automatic mode...\n";
                    break;
                }

                if (command == "q") {
                    std::cout << "\nSimulation stopped.\n";
                    return 0;
                }
            }
        }

        std::cout << "\nRunning simulation...\n";

        SimulationResult result = simulation.run();

        resultView.render(result, std::cout);

        if (result.totalGenerated !=
            result.totalServed + result.totalRejected) {
            std::cerr << "\nWarning: request count mismatch!\n";
            return 1;
        }

        std::cout << "\nSimulation completed successfully.\n";

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << '\n';
        return 1;
    }
}