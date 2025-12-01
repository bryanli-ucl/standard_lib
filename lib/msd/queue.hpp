#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace msd {

template <typename T>
class queue {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    data_t* _data;

    size_t _size;
    size_t _capacity;

    size_t _head;
    size_t _tail;

    void resize(size_t cap) {
        if (cap == _capacity) return;

        data_ptr ndata = new data_t[cap];
        for (size_t i = 0; i < _size; i++) {
            ndata[i] = _data[(_head + i) % _capacity];
        }
        delete[] _data;
        _data     = ndata;
        _capacity = cap;
        _head     = 0;
        _tail     = _size;
    }

    public:
    queue(const size_t cap = 64) {
        _capacity = cap;
        _data     = new data_t[_capacity];
        _size     = 0;
        _head     = 0;
        _tail     = 0;
    }

    virtual ~queue() { delete[] _data; }

    queue(const queue& other) {
        _capacity = other._capacity;
        _size     = other._size;
        _head     = other._head;
        _tail     = other._tail;
        _data     = new data_t[_capacity];
        for (size_t i = 0; i < _capacity; ++i) {
            _data[i] = other._data[i];
        }
    }

    queue& operator=(const queue& other) {
        if (this == &other) return *this;

        delete[] _data;
        _capacity = other._capacity;
        _size     = other._size;
        _head     = other._head;
        _tail     = other._tail;
        _data     = new data_t[_capacity];
        for (size_t i = 0; i < _capacity; i++) {
            _data[i] = other._data[i];
        }
        return *this;
    }

    void push_front(data_t val) {
        if (full()) resize(_capacity * 2);

        _head        = ((_head - 1) + _capacity) % _capacity;
        _data[_head] = val;
        _size        = _size + 1;
    }

    void push_back(data_t val) {
        if (full()) resize(_capacity * 2);

        _data[_tail] = val;
        _tail        = (_tail + 1) % _capacity;
        _size        = _size + 1;
    }

    void pop_front() {
        if (empty()) return;

        _head = (_head + 1) % _capacity;
        _size--;
    }

    void pop_back() {
        if (empty()) return;

        _tail = (_tail - 1) % _capacity;
        _size--;
    }

    data_t& front() const { return _data[_head]; }
    data_t& back() const { return _data[((_tail - 1) + _capacity) % _capacity]; }

    void clear() { _tail = 0, _head = 0, _size = 0; }
    bool empty() const { return _size == 0; }
    bool full() const { return _size == _capacity; }

    size_t capacity() const { return _capacity; }
    size_t size() const { return _size; }
};

} // namespace msd
