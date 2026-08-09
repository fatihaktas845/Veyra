#include "Apic.hpp"
#include "Msr.hpp"
#include "Io.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_tsc_frequency_request tsc_frequency_request = {
    .id = LIMINE_TSC_FREQUENCY_REQUEST_ID,
    .revision = 0,
    .response = nullptr
};

namespace {
    [[nodiscard]] inline uint32_t calculateApicTimerInitCount() {
        const uint8_t gate = Io::inb(0x61);
        Io::outb(0x61, (gate & 0xDC) | 0x01);

        Io::outb(0x43, 0xB0);

        const uint16_t pitTicks = 1193;
        Io::outb(0x42, static_cast<uint8_t>(pitTicks & 0xFF));
        Io::outb(0x42, static_cast<uint8_t>((pitTicks >> 8) & 0xFF));
        Msr::write(IA32_X2APIC_INIT_COUNT_MSR, 0xFFFFFFFFULL);

        while ((Io::inb(0x61) & 0x20) == 0);

        const uint32_t currentApicTimerCount = static_cast<uint32_t>(Msr::read(IA32_X2APIC_CURR_COUNT_MSR));

        Io::outb(0x61, gate);

        return 0xFFFFFFFF - currentApicTimerCount;
    }
}

void Apic::init() {
    uint64_t apicBase = Msr::read(IA32_APIC_BASE_MSR);

    apicBase |= 1ULL << 11;
    Msr::write(IA32_APIC_BASE_MSR, apicBase);

    apicBase |= 1ULL << 10;
    Msr::write(IA32_APIC_BASE_MSR, apicBase);
    
    Msr::write(IA32_X2APIC_SIV_MSR, (1ULL << 8) | 0xFFULL);

    // LAPIC Timer
    Msr::write(IA32_X2APIC_DIV_CONF_MSR, 0x3ULL);
    Msr::write(IA32_X2APIC_LVT_TIMER_MSR, (1ULL << 17) | 0x20ULL);

    uint32_t tickCountFor1ms;
    /* if (tsc_frequency_request.response)
        tickCountFor1ms = tsc_frequency_request.response->frequency / 16000;
    else */
        tickCountFor1ms = calculateApicTimerInitCount();
    
    Msr::write(IA32_X2APIC_INIT_COUNT_MSR, static_cast<uint64_t>(tickCountFor1ms));
}