#pragma once

#include "msddef.hpp"

#include "memory_pool.hpp"

namespace msd {

// 通用内存池分配器
template <typename T>
class PoolAllocator {
    private:
    MemoryPool* pool;

    public:
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    template <typename U>
    struct rebind {
        using other = PoolAllocator<U>;
    };

    // 默认构造函数（使用全局内存池）
    PoolAllocator() noexcept
    : pool(nullptr) {}

    // 从内存池构造
    explicit PoolAllocator(MemoryPool& p) noexcept
    : pool(&p) {}

    // 拷贝构造函数
    template <typename U>
    PoolAllocator(const PoolAllocator<U>& other) noexcept
    : pool(other.get_pool()) {}

    // 分配内存
    pointer allocate(size_type n) {
        if (!pool) {
            // 如果没有指定内存池，使用默认new
            return static_cast<pointer>(::operator new(n * sizeof(T)));
        }

        void* ptr = pool->allocate(n * sizeof(T));
        if (!ptr) {
            throw std::bad_alloc();
        }
        return static_cast<pointer>(ptr);
    }

    // 释放内存
    void deallocate(pointer p, size_type n) noexcept {
        if (!pool) {
            ::operator delete(p);
        } else {
            pool->deallocate(p);
        }
    }

    // 构造对象
    template <typename... Args>
    void construct(pointer p, Args&&... args) {
        ::new (static_cast<void*>(p)) T(forward<Args>(args)...);
    }

    // 析构对象
    void destroy(pointer p) {
        p->~T();
    }

    // 获取底层内存池
    MemoryPool* get_pool() const noexcept { return pool; }
};

// 比较操作符
template <typename T1, typename T2>
bool operator==(const PoolAllocator<T1>& lhs, const PoolAllocator<T2>& rhs) noexcept {
    return lhs.get_pool() == rhs.get_pool();
}

template <typename T1, typename T2>
bool operator!=(const PoolAllocator<T1>& lhs, const PoolAllocator<T2>& rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace msd