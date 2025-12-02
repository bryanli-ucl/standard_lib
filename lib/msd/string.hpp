#pragma once

#include "avr-def.hpp"

#include "iterator.hpp"
#include "move.hpp"

namespace msd {
template <typename T>
class basic_string {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    data_ptr _data;
    size_t _size;
    size_t _capacity;

    public:
    // default construction
    constexpr basic_string() noexcept
    : _data(nullptr), _size(), _capacity() {}

    basic_string(const data_ptr str)
    : _size() {
        while (str[_size] != data_t{}) size++;
        _capacity = _size + 1;
        _data     = static_cast<data_t>(::operator new(_capacity * sizeof(data_t)));

        for (size_t i = 0; i < _size; i++)
            ::new (&_data[i]) data_t(str[i]);

        ::new (&_data[_size]) data_t();
    }

    // copy construction
    basic_string(const basic_string& other) {
        _data     = static_cast<data_t>(::operator new(other._capacity * sizeof(data_t)));
        _size     = other._size;
        _capacity = other._capacity;

        try {
            for (size_t i = 0; i < _size; i++)
                ::new (&_data[i]) data_t(other.data[i]);
        } catch (...) {
            for (size_t i = 0; i < _size; i++)
                _data[i].~data_t();
            ::operator delete(_data);
            throw;
        }
    }

    // move construction
    basic_string(basic_string&& other) noexcept {
        _data     = other._data;
        _size     = other._size;
        _capacity = other._capacity;

        other._data     = nullptr;
        other._capacity = 0;
        other._size     = 0;
    }

    basic_string& operator=(basic_string&& other) noexcept {
        if (this != &other) {
            __destroy_all();
            _data     = other._data;
            _size     = other._size;
            _capacity = other._capacity;

            other._data     = nullptr;
            other._size     = 0;
            other._capacity = 0;
        }
        return *this;
    }

    // assignment from C-string
    basic_string& operator=(const data_t* str) {
        basic_string temp(str);
        swap(temp);
        return *this;
    }

    void swap(basic_string& other) noexcept {
        data_ptr temp_data   = _data;
        size_t temp_size     = _size;
        size_t temp_capacity = _capacity;

        _data     = other._data;
        _size     = other._size;
        _capacity = other._capacity;

        other._data     = temp_data;
        other._size     = temp_size;
        other._capacity = temp_capacity;
    }

    // list construction
    virtual ~basic_string() { __destroy_all(); }

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
    const char* c_str() const noexcept { return _data; }

    // size
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }
    bool empty() const noexcept { return _size == 0; }

    void clear() noexcept {
        for (size_t i = 0; i < _size; i++)
            _data[i].~data_t();
        _size = 0;
    }


    void reserve(size_t new_capacity) {
        if (new_capacity <= _capacity) return;

        size_t n_cap    = new_capacity + 1; // +1 for null terminator
        data_ptr n_data = static_cast<data_ptr>(::operator new(n_cap * sizeof(data_t)));
        size_t n_size   = 0;

        try {
            for (size_t i = 0; i < _size; i++) {
                ::new (&n_data[i]) data_t(msd::move(_data[i]));
                n_size++;
            }
            ::new (&n_data[n_size]) data_t(); // null terminator
        } catch (...) {
            for (size_t i = 0; i < n_size; i++)
                n_data[i].~data_t();
            ::operator delete(n_data);
            throw;
        }

        __destroy_all();
        _data     = n_data;
        _size     = n_size;
        _capacity = n_cap - 1; // store capacity without null terminator
    }

    // += operators
    basic_string& operator+=(const basic_string& str) {
        return append(str);
    }

    basic_string& operator+=(const data_t* str) {
        return append(str);
    }

    basic_string& operator+=(data_t ch) {
        push_back(ch);
        return *this;
    }

    // append functions
    basic_string& append(const basic_string& str) {
        return append(str._data, str._size);
    }

    basic_string& append(const data_t* str) {
        size_t len = 0;
        while (str[len] != data_t()) len++;
        return append(str, len);
    }

    basic_string& append(const data_t* str, size_t count) {
        if (_size + count >= _capacity) {
            reserve((_size + count) * 2);
        }

        for (size_t i = 0; i < count; i++) {
            ::new (&_data[_size + i]) data_t(str[i]);
        }
        _size += count;
        ::new (&_data[_size]) data_t(); // null terminator
        return *this;
    }

    basic_string& append(size_t count, data_t ch) {
        if (_size + count >= _capacity) {
            reserve((_size + count) * 2);
        }

        for (size_t i = 0; i < count; i++) {
            ::new (&_data[_size + i]) data_t(ch);
        }
        _size += count;
        ::new (&_data[_size]) data_t(); // null terminator
        return *this;
    }

    private:
    void __destroy_all() noexcept {
        clear();
        if (_data)
            ::operator delete(_data);
    }
};
using string = basic_string<char>;
} // namespace msd
