#pragma once

#include <stdint.h>

class Psf1Parser {
    private:
        const uint8_t* fontFile = nullptr;
        uint64_t L1[32] = {};

        struct Header {
            uint8_t magic[2];
            uint8_t mode;
            uint8_t charsize;
        };

    public:
        explicit Psf1Parser(const uint8_t* file);

        uint32_t decodeUtf8(const char* c);
        const uint8_t* getGlyphData(uint32_t codepoint);
};