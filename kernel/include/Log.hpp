#pragma once

#include <stdint.h>

namespace Log {
    void init();
    void print(const char* str, uint32_t color = 0x00FFFFFF);
}