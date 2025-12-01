#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <new>

#include "iterator.hpp"
#include "move.hpp"

namespace msd {
template <typename T>
class vector {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    data_ptr _data;
    size_t _size;
    size_t _capacity;

    public:
    // default construction
    constexpr vector() noexcept
    : _data(nullptr), _size(), _capacity() {}

    // copy construction
    vector(const vector& other) {
        _data     = static_cast<data_ptr>(::operator new(other._capacity * sizeof(data_t)));
        _size     = other._size;
        _capacity = other._capacity;

        try {
            for (size_t i = 0; i < _size; i++)
                ::new (&_data[i]) data_t(other._data[i]);
        } catch (...) {
            for (size_t i = 0; i < _size; i++)
                _data[i].~data_t();
            ::operator delete(_data);
            throw;
        }
    }
    vector& operator=(const vector& other) {
        if (this != &other) {
            vector vec(other);
            swap(vec);
        }
        return *this;
    }

    // move construction
    vector(vector&& other) noexcept {
        _data     = other._data;
        _size     = other._size;
        _capacity = other._capacity;

        other._data     = nullptr;
        other._capacity = 0;
        other._size     = 0;
    }

    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            __destory_all();

            _data     = other._data;
            _size     = other._size;
            _capacity = other._capacity;

            other._data     = nullptr;
            other._size     = 0;
            other._capacity = 0;
        }
        return *this;
    }

    virtual ~vector() { __destory_all(); }

    // access
    const data_t& at(size_t index) const {
        if (index >= _size)
            throw;
        return _data[index];
    }
    data_t& at(size_t index) {
        if (index >= _size)
            throw;
        return _data[index];
    }

    const data_t& operator[](size_t index) const { return at(index); }
    data_t& operator[](size_t index) { return at(index); }

    // iterator
    iterator<data_t> begin() noexcept { return iterator<data_t>{ _data }; }
    iterator<data_t> end() noexcept { return iterator<data_t>{ _data + _size }; }

    // raw data
    data_t* data() noexcept { return _data; }
    const data_t* data() const noexcept { return _data; }

    // size
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }
    bool empty() const noexcept { return _size == 0; }

    void clear() noexcept {
        for (size_t i = 0; i < _size; i++)
            _data[i].~data_t();
        _size = 0;
    }

    // control
    void pop_back() {
        if (empty()) throw;
        _data[_size - 1].~data_t();
        _size--;
    }

    void push_back(const data_t& val) { emplace_back(val); }
    void push_back(data_t&& val) { emplace_back(move(val)); }

    template <typename... Args>
    data_ref emplace_back(Args&&... args) {
        if (_size < _capacity) {
            size_t p = _size;
            ::new (&_data[p]) data_t(forward<Args>(args)...);
            _size++;
            return _data[p];
        }
        size_t n_cap    = (_capacity + 1) | (_capacity >> 1);
        size_t n_size   = 0;
        data_ptr n_data = static_cast<data_ptr>(::operator new(n_cap * sizeof(data_t)));

        try {
            for (size_t i = 0; i < _size; i++) {
                ::new (&n_data[i]) data_t(move(_data[i]));
                n_size++;
            }

            ::new (&n_data[n_size]) data_t(forward<Args>(args)...);
            n_size++;
        } catch (...) {
            for (size_t i = 0; i < _size; i++)
                n_data[i].~data_t();

            if (n_data)
                ::operator delete(n_data);
            throw;
        }

        __destory_all();

        _data     = n_data;
        _size     = n_size;
        _capacity = n_cap;

        return n_data[n_size];
    }

    private:
    void swap(vector& other) noexcept {
        data_ptr td = _data;
        size_t tsz  = _size;
        size_t tcap = _capacity;

        _data     = other._data;
        _size     = other._size;
        _capacity = other._capacity;

        other._data     = td;
        other._size     = tsz;
        other._capacity = tcap;
    }

    void __destory_all() noexcept {
        clear();
        if (_data)
            ::operator delete(_data);
    }
};
} // namespace msd
