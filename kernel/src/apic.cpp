#include "apic.hpp"
#include "msr.hpp"

void apic::init() {
    uint64_t apicBase = msr::read(IA32_APIC_BASE_MSR);

    apicBase |= (1 << 11) | (1 << 10);

    msr::write(IA32_APIC_BASE_MSR, apicBase);
    
    msr::write(IA32_X2APIC_SIV_MSR, (1 << 8) | 0xFF);

    // LAPIC Timer
    msr::write(IA32_X2APIC_DCR_MSR, 0x3);
    msr::write(IA32_X2APIC_LVT_MSR, (1 << 17) | 0x20);
    msr::write(IA32_X2APIC_ICR_MSR, 20'000'000);
}