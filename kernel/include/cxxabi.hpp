#pragma once

#include <stdint.h>

void call_global_constructors();

void* operator new(uint64_t size);
void* operator new[](uint64_t size);

extern "C" void __cxa_pure_virtual();
