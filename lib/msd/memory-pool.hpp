#pragma once

#include "avr-def.hpp"

#include "avr-memory.hpp"

namespace msd {
template <size_t BlockNumber, size_t BlockSize, typename T = uint8_t>
class MemoryPool {

    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    /*
    block structure:
    --------------------------------------
    | *next   |     data[BlockSize]      |
    | pointer |     array (pointer)      |
    | 2 bytes |   size depends on user   |
    --------------------------------------
    */
    struct block_t {
        block_t* next;
        T data[BlockSize];
    };
    using block_ptr = block_t*;
    using block_ref = block_t&;

    private:
    data_t m_memory[BlockNumber * sizeof(block_t)];
    block_ptr m_freelist;

    size_t m_free_count;
    size_t m_used_count;

    public:
    MemoryPool(const MemoryPool&)            = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool() noexcept : m_free_count(BlockNumber), m_used_count(0) {
        m_freelist = reinterpret_cast<block_ptr>(m_memory);

        for (size_t i = 0; i < BlockNumber - 1; i++) {
            block_ptr p = reinterpret_cast<block_ptr>(
            m_memory + i * sizeof(block_t));

            block_ptr n = reinterpret_cast<block_ptr>(
            m_memory + (i + 1) * sizeof(block_t));

            p->next = n;
        }
        reinterpret_cast<block_ptr>(
        m_memory + BlockNumber * sizeof(block_t))
        ->next = nullptr;
    }
    ~MemoryPool();

    void* allocate() noexcept {
        // no enough space
        if (m_freelist == nullptr) return nullptr;

        // grub 1 block from free list
        block_ptr b = m_freelist;
        m_freelist  = m_freelist->next;
        return b->data;
    }

    void deallocate(void* ptr) noexcept {
        if (ptr == nullptr) return;

        // put that block into free list
        block_ptr b = reinterpret_cast<block_ptr>(
        static_cast<data_ptr>(ptr) - offsetof(block_t, data));
        b->next    = m_freelist;
        m_freelist = b;
    }

    inline size_t free_count() const noexcept { return m_free_count; }
    inline size_t used_count() const noexcept { return m_used_count; }
    inline size_t block_size() const noexcept { return BlockSize; }
    inline size_t block_number() const noexcept { return BlockNumber; }
};
} // namespace msd
