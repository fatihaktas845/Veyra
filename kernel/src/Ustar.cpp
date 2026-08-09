#include "Ustar.hpp"
#include "Kstd.hpp"

#define BLOCK_SIZE 512
#define ALIGN_UP(value) (((value) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

VirtualAddress Ustar::readFile(VirtualAddress tarFile, const char* fileName) {
    const uint8_t* currentAddress = tarFile.asPtr<const uint8_t>();

    FileHeader fileEnd[2] = {};

    while (Kstd::memcmp(currentAddress, &fileEnd, sizeof(fileEnd)) != 0) {
        const FileHeader* header = reinterpret_cast<const FileHeader*>(currentAddress);
        currentAddress += BLOCK_SIZE;

        uint64_t fileSize = Kstd::strToUint64(header->fileSize, 8);

        if (Kstd::strcmp(header->fileName, fileName) != 0) {
            fileSize = ALIGN_UP(fileSize);
            currentAddress += fileSize;
            continue;
        }

        return VirtualAddress(reinterpret_cast<uint64_t>(currentAddress));
    }

    return VirtualAddress(0);
}