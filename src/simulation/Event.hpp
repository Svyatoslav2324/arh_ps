#pragma once

enum class EventKind {
    ARRIVAL,
    RELEASE
};

struct Event {
    double time;
    EventKind kind;
    int entityId;

    Event(double time, EventKind kind, int entityId):
        time(time),
        kind(kind),
        entityId(entityId)
    {}
};