#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace msd {
template <typename T, size_t N>
class array {
    using data_t = T;

    private:
    data_t _data[N];

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

    data_t& at(size_t p) {
        return _data[p];
    }

    const data_t& at(size_t p) const {
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
