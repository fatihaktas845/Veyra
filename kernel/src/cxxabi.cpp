#include "cxxabi.hpp"
#include "KernelHeap.hpp"

extern "C" {
	extern uint64_t __init_array_start;
	extern uint64_t __init_array_end;
}

void call_global_constructors() {
	uint64_t* start_ptr = &__init_array_start;

	while (start_ptr < &__init_array_end) {
		void (*func)() = reinterpret_cast<void(*)()>(*start_ptr++);
		func();
	}
}

void* operator new(uint64_t size) {
	return reinterpret_cast<void*>(KernelHeap::alloc(size).raw);
}

void* operator new[](uint64_t size) {
	return reinterpret_cast<void*>(KernelHeap::alloc(size).raw);
}

extern "C" void __cxa_pure_virtual() {
	while (1)
		__asm__ volatile("hlt");
}
