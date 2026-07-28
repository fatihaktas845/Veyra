#include "apic.hpp"
#include "msr.hpp"

void apic::init() {
    uint64_t apicBase = msr::read(IA32_APIC_BASE_MSR);

    apicBase |= 1ULL << 10;
    msr::write(IA32_APIC_BASE_MSR, apicBase);

    apicBase |= 1ULL << 11;
    msr::write(IA32_APIC_BASE_MSR, apicBase);
    
    msr::write(IA32_X2APIC_SIV_MSR, (1ULL << 8) | 0xFFULL);

    // LAPIC Timer
    msr::write(IA32_X2APIC_DIV_CONF_MSR, 0x3ULL);
    msr::write(IA32_X2APIC_LVT_TIMER_MSR, (1ULL << 17) | 0x20ULL);
    msr::write(IA32_X2APIC_INIT_COUNT_MSR, 20'000'000ULL);
}