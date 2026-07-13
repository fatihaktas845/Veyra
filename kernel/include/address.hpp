#pragma once

#include <stdint.h>

struct VirtualAddress;

struct PhysicalAddress {
	uint64_t raw = 0;

	PhysicalAddress();
	explicit PhysicalAddress(const uint64_t addr);

	VirtualAddress toVirtualAddress() const;
};

struct VirtualAddress {
	uint64_t raw = 0;

	VirtualAddress();
	explicit VirtualAddress(const uint64_t addr);

	PhysicalAddress toPhysicalAddress() const;

	template<typename T = void>
	T* asPtr() const {
		return reinterpret_cast<T*>(this->raw);
	}
};
