cmake_minimum_required(VERSION 3.16)

project(SMOSimulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(smo_simulator
    src/main.cpp

    src/dispatching/PlacementDispatcher.cpp
    src/dispatching/SelectionDispatcher.cpp

    src/simulation/EventCalendar.cpp
    src/simulation/Simulation.cpp
    src/simulation/SimulationConfig.cpp

    src/SMOelements/MonitoringSource.cpp
    src/SMOelements/QueueBuffer.cpp
    src/SMOelements/TelemetryRequest.cpp
    src/SMOelements/Worker.cpp

    src/statistic/ResultView.cpp
    src/statistic/StatisticsCollector.cpp
    src/statistic/StepView.cpp
)

target_include_directories(smo_simulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dispatching
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SMOelements
    ${CMAKE_CURRENT_SOURCE_DIR}/src/statistic
)

target_compile_options(smo_simulator PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
)