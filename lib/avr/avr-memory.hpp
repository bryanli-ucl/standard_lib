#pragma once

#include <avr-def.hpp>

#include <stdlib.h>

inline void* operator new(size_t size) {
    void* ptr = malloc(size);
    return ptr;
}
inline void* operator new[](size_t size) { return operator new(size); }

inline void operator delete(void* ptr) noexcept {
    if (ptr)
        free(ptr);
};
inline void operator delete[](void* ptr) noexcept { operator delete(ptr); }

inline void* operator new(size_t _, void* ptr) { return ptr; }
inline void* operator new[](size_t _, void* ptr) { return ptr; }
