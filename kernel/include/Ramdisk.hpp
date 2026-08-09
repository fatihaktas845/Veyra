#pragma once

#include <stdint.h>

namespace Ramdisk {
    const uint8_t* readFile(const char* fileName);
    uint64_t getFileSize(const char* fileName);
}