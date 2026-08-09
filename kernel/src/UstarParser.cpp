#include "UstarParser.hpp"
#include "Kstd.hpp"

#define BLOCK_SIZE 512
#define ALIGN_UP(value) (((value) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

UstarParser::UstarParser(const uint8_t* ustarFile) : file(ustarFile) {

}

const uint8_t* UstarParser::readFile(const char* fileName) {
    if (fileName[0] == '\0')
        return nullptr;
    
    const uint8_t* currentAddress = this->file;

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

        return currentAddress;
    }

    return nullptr;
}

uint64_t UstarParser::getFileSize(const char* fileName) {
    if (fileName[0] == '\0')
        return 0;
    
    const uint8_t* currentAddress = this->file;

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

        return fileSize;
    }

    return 0;
}