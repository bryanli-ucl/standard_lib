#pragma once

#include "msddef.hpp"

#include "iterator.hpp"
#include "move.hpp"

namespace msd {
template <typename T>
class vector {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    data_ptr m_data;
    size_t m_size;
    size_t _capacity;

    public:
    // default construction
    constexpr vector() noexcept
    : m_data(nullptr), m_size(), _capacity() {}

    // copy construction
    vector(const vector& other) {
        m_data    = static_cast<data_ptr>(::operator new(other._capacity * sizeof(data_t)));
        m_size    = other.m_size;
        _capacity = other._capacity;

        try {
            for (size_t i = 0; i < m_size; i++)
                ::new (&m_data[i]) data_t(other.m_data[i]);
        } catch (...) {
            for (size_t i = 0; i < m_size; i++)
                m_data[i].~data_t();
            ::operator delete(m_data);
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
        m_data    = other.m_data;
        m_size    = other.m_size;
        _capacity = other._capacity;

        other.m_data    = nullptr;
        other._capacity = 0;
        other.m_size    = 0;
    }

    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            __destory_all();

            m_data    = other.m_data;
            m_size    = other.m_size;
            _capacity = other._capacity;

            other.m_data    = nullptr;
            other.m_size    = 0;
            other._capacity = 0;
        }
        return *this;
    }

    virtual ~vector() { __destory_all(); }

    // access
    const data_t& at(size_t index) const {
        if (index >= m_size)
            throw;
        return m_data[index];
    }
    data_t& at(size_t index) {
        if (index >= m_size)
            throw;
        return m_data[index];
    }

    const data_t& operator[](size_t index) const { return at(index); }
    data_t& operator[](size_t index) { return at(index); }

    // iterator
    iterator<data_t> begin() noexcept { return iterator<data_t>{ m_data }; }
    iterator<data_t> end() noexcept { return iterator<data_t>{ m_data + m_size }; }

    // raw data
    data_t* data() noexcept { return m_data; }
    const data_t* data() const noexcept { return m_data; }

    // size
    size_t size() const noexcept { return m_size; }
    size_t capacity() const noexcept { return _capacity; }
    bool empty() const noexcept { return m_size == 0; }

    void clear() noexcept {
        for (size_t i = 0; i < m_size; i++)
            m_data[i].~data_t();
        m_size = 0;
    }

    // control
    void pop_back() {
        if (empty()) throw;
        m_data[m_size - 1].~data_t();
        m_size--;
    }

    void push_back(const data_t& val) { emplace_back(val); }
    void push_back(data_t&& val) { emplace_back(move(val)); }

    template <typename... Args>
    data_ref emplace_back(Args&&... args) {
        if (m_size < _capacity) {
            size_t p = m_size;
            ::new (&m_data[p]) data_t(forward<Args>(args)...);
            m_size++;
            return m_data[p];
        }
        size_t n_cap    = (_capacity + 1) | (_capacity >> 1);
        size_t n_size   = 0;
        data_ptr n_data = static_cast<data_ptr>(::operator new(n_cap * sizeof(data_t)));

        try {
            for (size_t i = 0; i < m_size; i++) {
                ::new (&n_data[i]) data_t(move(m_data[i]));
                n_size++;
            }

            ::new (&n_data[n_size]) data_t(forward<Args>(args)...);
            n_size++;
        } catch (...) {
            for (size_t i = 0; i < m_size; i++)
                n_data[i].~data_t();

            if (n_data)
                ::operator delete(n_data);
            throw;
        }

        __destory_all();

        m_data    = n_data;
        m_size    = n_size;
        _capacity = n_cap;

        return n_data[n_size];
    }

    private:
    void swap(vector& other) noexcept {
        data_ptr td = m_data;
        size_t tsz  = m_size;
        size_t tcap = _capacity;

        m_data    = other.m_data;
        m_size    = other.m_size;
        _capacity = other._capacity;

        other.m_data    = td;
        other.m_size    = tsz;
        other._capacity = tcap;
    }

    void __destory_all() noexcept {
        clear();
        if (m_data)
            ::operator delete(m_data);
    }
};
} // namespace msd
