#pragma once
#include "Event.hpp"
#include <queue>
#include <vector>

struct EventCompare {
    bool operator()(const Event& a, const Event& b) const;
};

class EventCalendar {
public:
    void schedule(const Event& event);
    Event popNext();
    bool isEmpty() const;
    std::size_t size() const;
    void removeArrivals();
    void clear();
private:
    std::priority_queue<Event, std::vector<Event>, EventCompare> events_;
};