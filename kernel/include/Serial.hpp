#pragma once

#include <stdint.h>

namespace Serial {
    extern uint16_t COM1_PORT;

    void write(const char* str);
}