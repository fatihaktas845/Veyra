#pragma once

#include <stdint.h>

namespace Ramdisk {
    const void* readFile(const char* fileName, uint64_t* fileSize_out);
}