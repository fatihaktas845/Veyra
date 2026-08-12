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

        uint32_t decodeUtf8(const char* c, uint8_t& charSize_out);
        uint32_t getGlyphIndex(uint32_t codepoint);

    public:
        uint8_t charsize = 0;
        
        explicit Psf1Parser(const uint8_t* file);

        Psf1Parser(const Psf1Parser&) = delete;
        Psf1Parser& operator=(const Psf1Parser&) = delete;

        Psf1Parser(Psf1Parser&&) = delete;
        Psf1Parser& operator=(Psf1Parser&&) = delete;

        const uint8_t* getGlyphData(const char* c, uint8_t& charSize_out);
};