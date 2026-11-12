#include "QueueBuffer.hpp"
#include <stdexcept>

QueueBuffer::QueueBuffer(std::size_t capacity)
    : capacity_(capacity)
{
}

bool QueueBuffer::isFull() const {
    return requests_.size() >= capacity_;
}

bool QueueBuffer::isEmpty() const {
    return requests_.empty();
}

std::size_t QueueBuffer::size() const {
    return requests_.size();
}

void QueueBuffer::append(int requestId) {
    if (isFull()) {
        throw std::overflow_error("Buffer is full");
    }

    requests_.push_back(requestId);
}

int QueueBuffer::removeOldest() {
    if (isEmpty()) {
        throw std::out_of_range("Buffer is empty");
    }

    int requestId = requests_.front();
    requests_.pop_front();

    return requestId;
}

int QueueBuffer::removeNewest() {
    if (isEmpty()) {
        throw std::out_of_range("Buffer is empty");
    }

    int requestId = requests_.back();
    requests_.pop_back();

    return requestId;
}

void QueueBuffer::clear() {
    requests_.clear();
}