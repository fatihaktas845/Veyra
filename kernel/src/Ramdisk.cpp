#include "Ramdisk.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST_ID,
    .revision = 0,
    .response = nullptr,
    .internal_module_count = 0,
    .internal_modules = nullptr
};

const void* Ramdisk::readFile(const char* fileName, uint64_t* fileSize_out) {
    (void)fileName;
    (void)fileSize_out;

    return nullptr;

    // Not Sterted Yet!!!
}