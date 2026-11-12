#pragma once

#include <cstddef>
#include <deque>

class QueueBuffer {
public:
    explicit QueueBuffer(std::size_t capacity);

    bool isFull() const;
    bool isEmpty() const;
    std::size_t size() const;

    void append(int requestId);

    int removeOldest();
    int removeNewest();

    void clear();

private:
    std::size_t capacity_;
    std::deque<int> requests_;
};