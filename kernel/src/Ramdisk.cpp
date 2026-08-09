#include "Ramdisk.hpp"
#include "UstarParser.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST_ID,
    .revision = 0,
    .response = nullptr,

    // Not Usable for Revision 0
    .internal_module_count = 0,
    .internal_modules = nullptr
};

const uint8_t* Ramdisk::readFile(const char* fileName) {
    void* ramdiskAddress = module_request.response->modules[0]->address;
    UstarParser parser(reinterpret_cast<const uint8_t*>(ramdiskAddress));

    return parser.readFile(fileName);
}

uint64_t Ramdisk::getFileSize(const char* fileName) {
    void* ramdiskAddress = module_request.response->modules[0]->address;
    UstarParser parser(reinterpret_cast<const uint8_t*>(ramdiskAddress));

    return parser.getFileSize(fileName);
}