#include "Log.hpp"
#include "Psf1Parser.hpp"
#include "Ramdisk.hpp"

extern volatile uint32_t* framebuffer_address;
extern uint64_t pitch;

namespace {
    Psf1Parser* parser = nullptr;
    uint64_t posX = 0;
    uint64_t posY = 0;
}

void Log::init() {
    parser = new Psf1Parser(Ramdisk::readFile("fonts/iso09.f16.psf"));
}

void Log::print(const char* str) {
    const uint8_t scale = 2;

    uint8_t charSize;
    while (*str != '\0') {
        if (*str == '\n') {
            posX = 0;
            posY += scale * parser->charsize;
            str++;
            continue;
        }

        const uint8_t* glyphData = parser->getGlyphData(str, charSize);

        for (uint8_t y = 0; y < parser->charsize; y++) {
            uint8_t row = glyphData[y];

            for (uint8_t x = 0; x < 8; x++) {
                if (row & (0x80 >> x)) {
                    for (uint8_t sy = 0; sy < scale; sy++) {
                        for (uint8_t sx = 0; sx < scale; sx++) {
                            const uint64_t coordX = posX + (x * scale) + sx;
                            const uint64_t coordY = posY + (y * scale) + sy;

                            framebuffer_address[coordY * (pitch / 4) + coordX] = 0x00FF00FF;
                        }
                    }
                }
            }
        }

        posX += scale * 8;
        str += charSize;
    }
}