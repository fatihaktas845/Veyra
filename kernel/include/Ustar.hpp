#pragma once

#include "Address.hpp"

namespace Ustar {
    struct FileHeader {
        char fileName[100];
        char fileMode[8];
        char uid[8];
        char gid[8];
        char fileSize[12];
        char mtime[12];
        char checksum[8];
        char typeFlag;
        char linkedFileName[100];
        char magic[6];
        char version[2];
        char uname[32];
        char gname[32];
        char major[8];
        char minor[8];
        char fileNamePrefix[155];
        char padding[12];
    } __attribute__((packed));

    VirtualAddress readFile(VirtualAddress tarFile, const char* fileName);
}