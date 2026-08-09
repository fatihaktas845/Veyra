#include "Ustar.hpp"
#include "Kstd.hpp"

#define BLOCK_SIZE 512
#define ALIGN_UP(value) (((value) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

const void* Ustar::readFile(void* tarFile, const char* fileName) {
    if (fileName[0] == '\0')
        return nullptr;
    
    const uint8_t* currentAddress = reinterpret_cast<const uint8_t*>(tarFile);

    FileHeader fileEnd[2] = {0};

    while (Kstd::memcmp(currentAddress, &fileEnd, sizeof(fileEnd)) != 0) {
        const FileHeader* header = reinterpret_cast<const FileHeader*>(currentAddress);
        currentAddress += BLOCK_SIZE;

        uint64_t fileSize = Kstd::strToUint64(header->fileSize, 8);

        if (Kstd::strcmp(header->fileName, fileName) != 0) {
            fileSize = ALIGN_UP(fileSize);
            currentAddress += fileSize;
            continue;
        }

        return reinterpret_cast<const void*>(currentAddress);
    }

    return nullptr;
}