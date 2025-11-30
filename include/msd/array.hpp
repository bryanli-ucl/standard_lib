#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "iterator.hpp"

namespace msd {
template <typename T, size_t N>
class array {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    data_t _data[N];

    void initializer(size_t index) {
        for (; index < N; index++) _data[index] = data_t{};
    }

    template <typename M, typename... Args>
    void initializer(size_t index, M val, Args... args) {
        if (index >= N) return;
        _data[index] = static_cast<data_t>(val);
        initializer(index + 1, args...);
    }

    public:
    array() : _data{} {}

    template <typename... Args>
    array(Args... args) {
        initializer(0, args...);
    }

    ~array() = default;

    data_t& operator[](const size_t index) {
        return _data[index];
    }

    const data_t& at(size_t p) const {
        if (p >= N) {
            p = N - 1;
        }
        return _data[p];
    }

    constexpr size_t size() const {
        return N;
    }

    constexpr size_t capacity() const {
        return N;
    }

    constexpr bool empty() const {
        return size() == 0;
    }

    size_t fill(data_t val) {
        for (size_t i = 0; i < N; i++) {
            _data[i] = val;
        }
        return N;
    }

    msd::iterator<data_t> begin() {
        return iterator{ _data };
    }

    msd::iterator<data_t> end() {
        return iterator{ _data + N };
    }

    bool operator==(const array& other) {
        if (other.size() != N) return false;

        for (size_t i = 0; i < N; i++) {
            if (_data[i] != other.at(i)) return false;
        }
        return true;
    }

    bool operator!=(const array& other) {
        return (*this == other);
    }
};

template <typename T, typename... Args>
array<T, sizeof...(Args) + 1> make_array(T arg1, Args... args) {
    return array<T, sizeof...(Args) + 1>{ static_cast<T>(arg1), static_cast<T>(args)... };
}

} // namespace msd
