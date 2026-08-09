#include "Address.hpp"
#include "Hhdm.hpp"

PhysicalAddress::PhysicalAddress() = default;

PhysicalAddress::PhysicalAddress(const uint64_t addr) : raw(addr) { }

VirtualAddress PhysicalAddress::toVirtualHhdmAddress() const {
	return VirtualAddress(this->raw + Hhdm::offset);
}


VirtualAddress::VirtualAddress() = default;

VirtualAddress::VirtualAddress(const uint64_t addr) : raw(addr) { }

PhysicalAddress VirtualAddress::toPhysicalHhdmAddress() const {
	return PhysicalAddress(this->raw - Hhdm::offset);
}
