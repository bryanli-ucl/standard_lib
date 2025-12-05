#pragma once

#include "avr-def.hpp"

#include <avr-memory.hpp>

#include "iterator.hpp"
#include "move.hpp"

namespace msd {
template <typename T, size_t MaxSize = 16>
class vector {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    data_t m_data[MaxSize];
    size_t m_size;
    size_t m_capacity;

    public:
    // default construction
    constexpr vector() noexcept
    : m_data{}, m_size(), m_capacity(MaxSize) {}

    // copy construction
    vector(const vector& other) {
        m_capacity = MaxSize;
        m_size     = (other.m_size > m_capacity) ? m_capacity : other.m_size;
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = other.m_data[i];
    }
    vector& operator=(const vector& other) {
        if (this != &other) {
            m_capacity = MaxSize;
            m_size     = (other.m_size > m_capacity) ? m_capacity : other.m_size;
            for (size_t i = 0; i < m_size; i++)
                m_data[i] = other.m_data[i];
        }
        return *this;
    }

    // move construction
    vector(vector&& other) noexcept {
        m_capacity = MaxSize;
        m_size     = (other.m_size > m_capacity) ? m_capacity : other.m_size;
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = move(other.m_data[i]);
        other.m_size = 0;
    }

    vector& operator=(vector&& other) noexcept {
        if (this == &other)
            return *this;

        this->clear();

        m_capacity = MaxSize;
        m_size     = (other.m_size > m_capacity) ? m_capacity : other.m_size;
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = move(other.m_data[i]);

        other.m_size = 0;
        return *this;
    }

    // destruction
    virtual ~vector() {
        for (size_t i = 0; i < m_size; i++) {
            m_data[i].~data_t();
        }
    }

    // access
    const data_t& at(size_t index) const { return m_data[index >= m_size ? 0 : index]; }
    data_t& at(size_t index) { return m_data[index >= m_size ? 0 : index]; }
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
    size_t capacity() const noexcept { return m_capacity; }
    bool empty() const noexcept { return m_size == 0; }

    void clear() noexcept {
        for (size_t i = 0; i < m_size; i++)
            m_data[i].~data_t();
        m_size = 0;
    }

    // control
    void pop_back() {
        if (empty()) return;
        m_data[m_size - 1].~data_t();
        m_size--;
    }

    void push_back(const data_t& val) {
        if (m_size == m_capacity) return;
        m_data[m_size++] = val;
    }
    void push_back(data_t&& val) {
        if (m_size == m_capacity) return;
        m_data[m_size++] = msd::move(val);
    }
};
} // namespace msd
