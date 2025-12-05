#pragma once

#include <avr-def.hpp>

#include "iterator.hpp"
#include "move.hpp"

namespace msd {
template <typename CharT, size_t N>
class string_basic {
    using data_t         = CharT;
    using data_ptr       = CharT*;
    using data_ref       = CharT&;
    using const_data_ptr = const CharT*;
    using const_data_ref = const CharT&;

    private:
    data_t m_data[N + 1];
    data_t m_null_data{};
    size_t m_size     = 0;
    size_t m_capacity = N;

    public:
    // =============== default construction ===================
    // basic destruction
    virtual ~string_basic() noexcept = default;
    // basic construction
    string_basic() noexcept : m_data{} { m_data[0] = data_t(); }


    // =============== copy construction ===================
    // copy construction with same N
    string_basic(const string_basic& other) : m_size(other.m_size) {
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = other.m_data[i];

        m_data[m_size] = data_t();
    }
    // copy construction with same N by operator
    string_basic& operator=(const string_basic& other) {
        if (this == &other)
            return *this;

        m_size = (other.m_size > m_capacity) ? m_capacity : other.m_size;

        for (size_t i = 0; i < m_size; i++)
            m_data[i] = other.m_data[i];

        m_data[m_size] = data_t();

        return *this;
    }

    // copy construction with different N
    template <size_t M>
    string_basic(const string_basic<data_t, M>& other) : m_size(0) {
        for (m_size = 0; m_size < m_capacity && m_size < other.size(); m_size++)
            m_data[m_size] = other[m_size];
        m_data[m_size] = data_t();
    }
    // copy construction with different N by operator
    template <size_t M>
    string_basic& operator=(const string_basic<CharT, M>& other) {
        if constexpr (M == N)
            if (this == &other)
                return *this;

        for (m_size = 0; m_size < m_capacity && m_size < other.size(); m_size++)
            m_data[m_size] = other[m_size];

        m_data[m_size] = m_null_data;
        return *this;
    }


    // =============== move construction ===================
    // move construction with same N
    string_basic(string_basic&& other) noexcept : m_size(other.m_size) {
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = msd::move(other.m_data[i]);

        m_data[m_size]  = data_t();
        other.m_size    = 0;
        other.m_data[0] = data_t();
    }
    // move construction with same N by operator
    string_basic& operator=(string_basic&& other) noexcept {
        if (this == &other)
            return *this;

        m_size = (other.m_size > m_capacity) ? m_capacity : other.m_size;
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = msd::move(other.m_data[i]);

        m_data[m_size]  = data_t();
        other.m_size    = 0;
        other.m_data[0] = data_t();

        return *this;
    }

    // move construction with different N
    template <size_t M>
    string_basic(string_basic<CharT, M>&& other) noexcept {
        for (m_size = 0; m_size < m_capacity && m_size < other.size(); m_size++)
            m_data[m_size] = msd::move(other[m_size]);

        m_data[m_size] = data_t();
        other.clear();
    }
    // move construction with different N by operator
    template <size_t M>
    string_basic& operator=(string_basic<CharT, M>&& other) noexcept {
        if constexpr (M == N)
            if (this == &other)
                return *this;

        for (m_size = 0; m_size < m_capacity && m_size < other.size(); m_size++)
            m_data[m_size] = msd::move(other[m_size]);

        m_data[m_size] = data_t();
        other.clear();
        return *this;
    }


    // =============== construct from cstr ===================
    // construct from a c-style string
    string_basic(const_data_ptr str) : m_size(0) {
        for (m_size = 0; m_size < m_capacity && str[m_size] != m_null_data; m_size++)
            m_data[m_size] = str[m_size];
        m_data[m_size] = m_null_data;
    }
    // construct from a c-style string by operator
    string_basic& operator=(const_data_ptr str) {
        for (m_size = 0; m_size < N && str[m_size] != data_t{}; m_size++)
            m_data[m_size] = str[m_size];
        m_data[m_size] = data_t();
        return *this;
    }

    // access
    data_ref operator[](size_t index) { return m_data[index]; }
    const_data_ref operator[](size_t index) const { return m_data[index]; }
    data_t& at(size_t index) { return (index >= m_size) ? m_null_data : m_data[index]; }
    const data_t& at(size_t index) const { return (index >= m_size) ? m_null_data : m_data[index]; }

    // c-style access
    data_ptr data() noexcept { return m_data; }
    const_data_ptr data() const noexcept { return m_data; }
    const_data_ptr c_str() const noexcept { return m_data; }

    // stats
    size_t size() const noexcept { return m_size; }
    size_t capacity() const noexcept { return m_capacity; }
    bool empty() const noexcept { return m_size == 0; }
    bool full() const noexcept { return m_size >= m_capacity; }
    void clear() noexcept { m_size = 0, m_data[0] = data_t(); }

    // elements control
    void push_back(data_t ch) {
        if (m_size >= m_capacity) return;
        m_data[m_size++] = ch;
        m_data[m_size]   = data_t();
    }
    void pop_back() {
        if (m_size == 0) return;
        m_data[--m_size] = data_t{};
    }

    // addition operator
    template <size_t M>
    string_basic& operator+=(const string_basic<data_t, M>& other) {
        for (size_t i = 0; i < other.size() && m_size < m_capacity; i++, m_size++)
            m_data[m_size] = other[i];

        m_data[m_size] = data_t();
        return *this;
    }

    string_basic& operator+=(const_data_ptr str) {
        for (size_t i = 0; str[i] != data_t{} && m_size < m_capacity; m_size++, i++)
            m_data[m_size] = str[i];

        m_data[m_size] = data_t();
        return *this;
    }
    
    string_basic& operator+=(data_t ch) {
        if (m_size >= N) return *this;

        m_data[m_size++] = ch;
        m_data[m_size]   = data_t();
        return *this;
    }

    // iterator
    msd::iterator<data_t> begin() noexcept { return msd::iterator<data_t>(m_data); }
    msd::iterator<data_t> end() noexcept { return msd::iterator<data_t>(m_data + m_size); }
    msd::iterator<const data_t> begin() const noexcept { return msd::iterator<const data_t>(m_data); }
    msd::iterator<const data_t> end() const noexcept { return msd::iterator<const data_t>(m_data + m_size); }
};

template <typename CharT, size_t N1, size_t N2>
string_basic<CharT, N1 + N2> operator+(const string_basic<CharT, N1>& lhs, const string_basic<CharT, N2>& rhs) {
    string_basic<CharT, N1 + N2> ret;

    for (size_t i = 0; i < lhs.size(); i++)
        ret.push_back(lhs[i]);
    for (size_t i = 0; i < rhs.size(); i++)
        ret.push_back(rhs[i]);

    return ret;
}

template <typename CharT, size_t N>
string_basic<CharT, N * 2> operator+(const string_basic<CharT, N>& lhs, const CharT* rhs) {
    string_basic<CharT, N * 2> ret = lhs;
    ret += rhs;
    return ret;
}

template <typename CharT, size_t N>
string_basic<CharT, N * 2> operator+(const CharT* lhs, const string_basic<CharT, N>& rhs) {
    string_basic<CharT, N * 2> ret;

    for (size_t i = 0; lhs[i] != CharT{} && ret.size() < N; i++)
        ret.push_back(lhs[i]);
    ret += rhs;
    return ret;
}

template <size_t N>
using string = string_basic<char, N>;

} // namespace msd
