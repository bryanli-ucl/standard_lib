#pragma once

#include "avr-def.hpp"

#include "memory_pool.hpp"

namespace msd {

template <typename T>
class PoolAllocator {
    private:
    MemoryPool* pool;

    public:
    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    template <typename U>
    struct rebind {
        using other = PoolAllocator<U>;
    };

    PoolAllocator() noexcept
    : pool(nullptr) {}

    explicit PoolAllocator(MemoryPool& p) noexcept
    : pool(&p) {}

    template <typename U>
    PoolAllocator(const PoolAllocator<U>& other) noexcept
    : pool(other.get_pool()) {}

    data_ptr allocate(size_type n) {
        void* ptr = pool->allocate(n * sizeof(T));
        if (ptr == nullptr) {
            ptr = malloc(n * sizeof(T));
        }
        return static_cast<data_ptr>(ptr);
    }

    void deallocate(data_ptr p, size_type n) noexcept {
        if (!pool) {
            ::operator delete(p);
        } else {
            pool->deallocate(p);
        }
    }

    template <typename... Args>
    void construct(data_ptr p, Args&&... args) {
        operator new(static_cast<void*>(p)) T(forward<Args>(args)...);
    }

    void destroy(data_ptr p) {
        p->~T();
    }

    MemoryPool* get_pool() const noexcept { return pool; }
};

template <typename T1, typename T2>
bool operator==(const PoolAllocator<T1>& lhs, const PoolAllocator<T2>& rhs) noexcept {
    return lhs.get_pool() == rhs.get_pool();
}

template <typename T1, typename T2>
bool operator!=(const PoolAllocator<T1>& lhs, const PoolAllocator<T2>& rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace msd