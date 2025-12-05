#pragma once

#include "msddef.hpp"

#include <stdlib.h>

inline void* operator new(size_t size) { return malloc(size); }
inline void* operator new[](size_t size) { return malloc(size); }

inline void operator delete(void* ptr) noexcept { free(ptr); };
inline void operator delete[](void* ptr) noexcept { free(ptr); }

inline void* operator new(size_t _, void* ptr) { return ptr; }
inline void* operator new[](size_t _, void* ptr) { return ptr; }
