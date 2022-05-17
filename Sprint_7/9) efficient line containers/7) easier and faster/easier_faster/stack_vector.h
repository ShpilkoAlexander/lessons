#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        size_ = a_size;
        if (a_size > N) {
            throw std::invalid_argument("");
        }
    }

    T& operator[](size_t index) {
        return container_[index];
    }
    const T& operator[](size_t index) const {
        return container_[index];
    }
    auto begin() {
        return container_.begin();
    }

    auto end() {
        return begin() + size_;
    }

    auto begin() const {
        return  container_.cbegin();
    }

    auto end() const {
        return begin() + size_;
    }

    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return container_.max_size();
    }

    void PushBack(const T& value) {
        if (size_ == Capacity()) {
            throw std::overflow_error("");
        }
        container_[empty_element_++] = value;
        ++size_;
    }

    T PopBack() {
        if (size_ == 0) {
            throw  std::underflow_error("");
        }
        --size_;
        return container_[--empty_element_];
    }

private:
    std::array<T,N> container_;
    size_t empty_element_ = 0;
    size_t size_ = 0;
};
