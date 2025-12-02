#pragma once

#include "msddef.hpp"

#include <stdlib.h>

void* operator new(size_t size) {
    void* ptr = malloc(size);
    return ptr;
}
void* operator new[](size_t size) { return operator new(size); }

void operator delete(void* ptr) noexcept {
    if (ptr)
        free(ptr);
};
void operator delete[](void* ptr) noexcept { operator delete(ptr); }

void* operator new(size_t _, void* ptr) { return ptr; }
void* operator new[](size_t _, void* ptr) { return ptr; }
