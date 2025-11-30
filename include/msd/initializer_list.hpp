#pragma once

namespace msd {
template <typename T>
class initializer_list {
    private:
    const T* _begin;
    const T* _end;

    public:
    constexpr initializer_list() noexcept : _begin(nullptr), _end(nullptr) {}

    constexpr initializer_list(const T* begin, const T* end) noexcept
    : _begin(begin), _end(end) {}

    constexpr size() const noexcept {
        return _end - _begin;
    }

    constexpr empty() const noexcept {
        return _end == _begin;
    }

    constexpr const T* begin() const noexcept {
        return _begin;
    }

    constexpr const T* end() const noexcept {
        return _end;
    }
};
} // namespace msd
