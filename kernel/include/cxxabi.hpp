#pragma once

#include <stdint.h>

void call_global_constructors();

void* operator new(uint64_t size);
void* operator new[](uint64_t size);

inline void* operator new(uint64_t, void* ptr) noexcept {
    return ptr;
}

inline void* operator new[](uint64_t, void* ptr) noexcept {
    return ptr;
}

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

// For C++14 and later!!!
void operator delete(void* ptr, uint64_t size) noexcept;
void operator delete[](void* ptr, uint64_t size) noexcept;

extern "C" void __cxa_pure_virtual();
