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
    iterator(data_ptr ptr = nullptr) noexcept
    : _ptr(ptr) {}
    iterator(const iterator& other) noexcept
    : _ptr(other._ptr) {}
    virtual ~iterator() = default;

    iterator& operator=(const iterator& other) noexcept {
        if (this != &other) _ptr = other._ptr;
        return *this;
    }

    // itor . / ->
    data_ref operator*() const noexcept { return *_ptr; }
    data_ptr operator->() const noexcept { return _ptr; }

    // itor ==/!= itor
    bool operator==(const iterator& other) noexcept { return _ptr == other._ptr; }
    bool operator!=(const iterator& other) noexcept { return _ptr != other._ptr; }

    // itor ++/-- (prefix)
    iterator& operator++() noexcept {
        ++_ptr;
        return *this;
    }
    iterator& operator--() noexcept {
        --_ptr;
        return *this;
    }

    // itor ++/-- (suffix)
    iterator operator++(int) noexcept {
        iterator tmp{ *this };
        ++_ptr;
        return tmp;
    }
    iterator operator--(int) noexcept {
        iterator tmp{ *this };
        --_ptr;
        return tmp;
    }
};
} // namespace msd
