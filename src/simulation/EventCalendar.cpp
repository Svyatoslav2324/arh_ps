#include "EventCalendar.hpp"
#include <stdexcept>

bool EventCompare::operator()(const Event& a, const Event& b) const {
    if (a.time != b.time) {
        return a.time > b.time;
    }

    if (a.kind != b.kind) {
        return a.kind == EventKind::ARRIVAL;
    }

    return false;
}

void EventCalendar::schedule(const Event& event) {
    events_.push(event);
}

Event EventCalendar::popNext() {
    if (events_.empty()) {
        throw std::out_of_range("calendar is empty");
    }

    auto event = events_.top();
    events_.pop();

    return event;
}

bool EventCalendar::isEmpty() const {
    return events_.empty();
}

std::size_t EventCalendar::size() const {
    return events_.size();
}

void EventCalendar::removeArrivals() {
    std::priority_queue<Event, std::vector<Event>, EventCompare> newEvents;

    while (!events_.empty()) {
        auto event = events_.top();
        events_.pop();

        if (event.kind == EventKind::RELEASE) {
            newEvents.push(event);
        }
    }

    events_.swap(newEvents);
}

void EventCalendar::clear() {
    while (!events_.empty()) {
        events_.pop();
    }
}