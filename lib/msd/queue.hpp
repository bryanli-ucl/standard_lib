#pragma once

// remove reference
template <typename T>
struct remove_reference {
    using type = T;
};
template <typename T>
struct remove_reference<T&> {
    using type = T;
};
template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

namespace msd {

template <typename T, size_t MaxSize = 16>
class queue {
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    data_t m_data[MaxSize];

    size_t m_size;
    size_t m_capacity;

    size_t m_head;
    size_t m_tail;

    public:
    queue() noexcept : m_size(0), m_capacity(MaxSize), m_head(0), m_tail(0) {}

    queue(const queue& other) noexcept {
        if (other.m_size > m_capacity) {
            queue();
            return;
        }
        m_size = other.m_size;
        m_head = other.m_head;
        m_tail = other.m_tail;
        for (size_t i = 0; i < m_capacity; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    queue& operator=(const queue& other) {
        if (this == &other) return *this;

        m_size = other.m_size;
        m_head = other.m_head;
        m_tail = other.m_tail;

        for (size_t i = 0; i < MaxSize; ++i) {
            m_data[i] = other.m_data[i];
        }

        return *this;
    }

    virtual ~queue() = default;

    void push_front(const data_t& val) {
        if (full()) {
            m_tail = ((m_tail - 1) + m_capacity) % m_capacity;
        } else {
            m_size = m_size + 1;
        }
        m_head         = ((m_head - 1) + m_capacity) % m_capacity;
        m_data[m_head] = val;
    }

    void push_back(data_t val) {
        if (full()) {
            m_head = ((m_head + 1) + m_capacity) % m_capacity;
        } else {
            m_size++;
        }
        m_data[m_tail] = val;
        m_tail         = ((m_tail + 1) + m_capacity) % m_capacity;
    }

    void pop_front() {
        if (empty()) return;

        m_head = (m_head + 1) % m_capacity;
        m_size--;
    }

    void pop_back() {
        if (empty()) return;

        m_tail = (m_tail - 1) % m_capacity;
        m_size--;
    }

    data_t& front() { return m_data[m_head]; }
    data_t& back() { return m_data[((m_tail - 1) + m_capacity) % m_capacity]; }

    void clear() { m_tail = 0, m_head = 0, m_size = 0; }
    bool empty() const { return m_size == 0; }
    bool full() const { return m_size == m_capacity; }

    size_t capacity() const { return m_capacity; }
    size_t size() const { return m_size; }
};

} // namespace msd
