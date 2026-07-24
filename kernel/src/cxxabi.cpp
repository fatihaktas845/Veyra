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
	if (size == 0)
		size = 1;

	return reinterpret_cast<void*>(KernelHeap::alloc(size).raw);
}

void* operator new[](uint64_t size) {
	return ::operator new(size);
}

void operator delete(void* ptr) noexcept {
	if (!ptr)
		return;
	
	const uint64_t addr = reinterpret_cast<uint64_t>(ptr);
	KernelHeap::free(VirtualAddress(addr));
}

void operator delete[](void* ptr) noexcept {
	::operator delete(ptr);
}

void operator delete(void* ptr, uint64_t size) noexcept {
	(void)size;
	::operator delete(ptr);
}

void operator delete[](void* ptr, uint64_t size) noexcept {
	(void)size;
	::operator delete(ptr);
}

extern "C" void __cxa_pure_virtual() {
	while (1)
		__asm__ volatile("hlt");
}
