#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace msd {
template <typename T>
class iterator {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    protected:
    data_ptr _ptr;

    public:
    iterator(data_ptr ptr = nullptr) : _ptr(ptr) {}
    iterator(const iterator& other) : _ptr(other._ptr) {}
    iterator& operator=(const iterator& other) {
        if (this != &other) _ptr = other._ptr;
        return *this;
    }

    // itor . / ->
    data_ref operator*() const {
        return *_ptr;
    }
    data_ptr operator->() const {
        return _ptr;
    }

    // itor ++/-- (prefix)
    iterator& operator++() {
        ++_ptr;
        return *this;
    }
    iterator& operator--() {
        --_ptr;
        return *this;
    }

    // itor ++/-- (suffix)
    iterator operator++(int) {
        iterator tmp{ *this };
        ++_ptr;
        return tmp;
    }
    iterator operator--(int) {
        iterator tmp{ *this };
        --_ptr;
        return tmp;
    }

    // itor ==/!= itor
    bool operator==(const iterator& other) {
        return _ptr == other._ptr;
    }
    bool operator!=(const iterator& other) {
        return _ptr != other._ptr;
    }

    ~iterator() = default;
};
} // namespace msd
