#include "Apic.hpp"
#include "Msr.hpp"
#include "Io.hpp"

namespace {
    [[nodiscard]] inline uint32_t calculateApicTimerInitCount() {
        const uint8_t gate = io::inb(0x61);
        io::outb(0x61, (gate & 0xDC) | 0x01);

        io::outb(0x43, 0xB0);

        const uint16_t pitTicks = 1193;
        io::outb(0x42, static_cast<uint8_t>(pitTicks & 0xFF));
        io::outb(0x42, static_cast<uint8_t>((pitTicks >> 8) & 0xFF));
        msr::write(IA32_X2APIC_INIT_COUNT_MSR, 0xFFFFFFFFULL);

        while ((io::inb(0x61) & 0x20) == 0);

        const uint32_t currentApicTimerCount = static_cast<uint32_t>(msr::read(IA32_X2APIC_CURR_COUNT_MSR));

        io::outb(0x61, gate);

        return 0xFFFFFFFF - currentApicTimerCount;
    }
}

void apic::init() {
    uint64_t apicBase = msr::read(IA32_APIC_BASE_MSR);

    apicBase |= 1ULL << 11;
    msr::write(IA32_APIC_BASE_MSR, apicBase);

    apicBase |= 1ULL << 10;
    msr::write(IA32_APIC_BASE_MSR, apicBase);
    
    msr::write(IA32_X2APIC_SIV_MSR, (1ULL << 8) | 0xFFULL);

    // LAPIC Timer
    msr::write(IA32_X2APIC_DIV_CONF_MSR, 0x3ULL);
    msr::write(IA32_X2APIC_LVT_TIMER_MSR, (1ULL << 17) | 0x20ULL);

    const uint32_t tickCountFor1ms = calculateApicTimerInitCount();
    msr::write(IA32_X2APIC_INIT_COUNT_MSR, static_cast<uint64_t>(tickCountFor1ms));
}