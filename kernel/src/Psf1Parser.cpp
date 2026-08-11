#include "Psf1Parser.hpp"
#include "Kstd.hpp"

#define PSF1_MODE512    0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04

Psf1Parser::Psf1Parser(const uint8_t* file) : fontFile(file) {
    const Header* header = reinterpret_cast<const Header*>(this->fontFile);
    const uint64_t glyphCount = header->mode & PSF1_MODE512 ? 512 : 256;
    const uint8_t* glyphDataStart = this->fontFile + sizeof(*header);
    const uint16_t* unicodeTableStart = reinterpret_cast<const uint16_t*>(glyphDataStart + header->charsize * glyphCount);

    uint32_t currentGlyphIndex = 0;

    while (currentGlyphIndex < glyphCount) {
        if (*unicodeTableStart == 0xFFFF) {
            currentGlyphIndex++;
            unicodeTableStart++;
            continue;
        } else if (*unicodeTableStart == 0xFFFE) {
            while (*unicodeTableStart != 0xFFFF)
                unicodeTableStart++;
            
            currentGlyphIndex++;
            unicodeTableStart++;
            continue;
        }

        const uint8_t l1_index = (*unicodeTableStart >> 16) & 0xFF;
        const uint8_t l2_index = (*unicodeTableStart >> 8) & 0xFF;
        const uint8_t l3_index = *unicodeTableStart & 0xFF;

        uint64_t* L2 = nullptr;
        uint32_t* L3 = nullptr;

        if (L1[l1_index] != 0)
            L2 = reinterpret_cast<uint64_t*>(L1[l1_index]);
        else {
            L2 = new uint64_t[256]{};
            L1[l1_index] = reinterpret_cast<uint64_t>(L2);
        }

        if (L2[l2_index] != 0)
            L3 = reinterpret_cast<uint32_t*>(L2[l2_index]);
        else {
            L3 = new uint32_t[256];
            Kstd::memset(L3, 0xFFFFFFFF, 256 * sizeof(uint32_t));
            L2[l2_index] = reinterpret_cast<uint64_t>(L3);
        }

        L3[l3_index] = currentGlyphIndex;

        unicodeTableStart++;
    }
}

uint32_t Psf1Parser::decodeUtf8(const char* c) {
    const uint8_t* s = reinterpret_cast<const uint8_t*>(c);

    if ((s[0] & 0x80) == 0) // 0xxxxxxx
        return s[0];
    else if ((s[0] & 0xE0) == 0xC0) // 110xxxxx 10xxxxxx
        return ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
    else if ((s[0] & 0xF0) == 0xE0) // 1110xxxx 10xxxxxx 10xxxxxx
        return ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
    else if ((s[0] & 0xF8) == 0xF0) // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        return ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
    else
        return -1;
}

uint32_t Psf1Parser::getGlyphIndex(uint32_t codepoint) {
    const uint8_t l1_index = (codepoint >> 16) & 0xFF;
    const uint8_t l2_index = (codepoint >> 8) & 0xFF;
    const uint8_t l3_index = codepoint & 0xFF;

    uint64_t* L2 = reinterpret_cast<uint64_t*>(L1[l1_index]);
    uint32_t* L3 = reinterpret_cast<uint32_t*>(L2[l2_index]);

    return L3[l3_index];
}

const uint8_t* Psf1Parser::getGlyphData(const char* c) {
    // Not Started Yet!!!
}