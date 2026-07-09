#include "cxxabi.hpp"

#include <stdint.h>

extern "C" {
	extern uint64_t __init_array_start;
	extern uint64_t __init_array_end;
}

void call_global_constructors() {
	uint64_t* start_ptr = reinterpret_cast<uint64_t*>(&__init_array_start);

	while (start_ptr < reinterpret_cast<uint64_t*>(&__init_array_end)) {
		void (*func)() = reinterpret_cast<void(*)()>(*start_ptr++);
		func();
	}
}

void __cxa_pure_virtual() {
	while (1)
		__asm__ volatile("hlt");
}
