#include "address.hpp"
#include "hhdm.hpp"

PhysicalAddress::PhysicalAddress() = default;

PhysicalAddress::PhysicalAddress(const uint64_t addr) : raw(addr) { }

VirtualAddress PhysicalAddress::toVirtualAddress() const {
	return VirtualAddress(this->raw + hhdm::offset);
}


VirtualAddress::VirtualAddress() = default;

VirtualAddress::VirtualAddress(const uint64_t addr) : raw(addr) { }

PhysicalAddress VirtualAddress::toPhysicalAddress() const {
	return PhysicalAddress(this->raw - hhdm::offset);
}
