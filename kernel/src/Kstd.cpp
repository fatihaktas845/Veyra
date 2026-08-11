#include "Kstd.hpp"
#include "Ramdisk.hpp"
#include "Psf1Parser.hpp"

void* Kstd::memset(void* dest, int c, uint64_t count) {
    uint8_t* d = static_cast<uint8_t*>(dest);
    uint8_t val = static_cast<uint8_t>(c);

    while (count > 0 && (reinterpret_cast<uintptr_t>(d) & 7) != 0) {
        *d++ = val;
        count--;
    }

    uint64_t val64 = static_cast<uint64_t>(val);
    val64 |= (val64 << 8);
    val64 |= (val64 << 16);
    val64 |= (val64 << 32);

    uint64_t* d64 = reinterpret_cast<uint64_t*>(d);
    while (count >= 8) {
        *d64++ = val64;
        count -= 8;
    }

    d = reinterpret_cast<uint8_t*>(d64);
    while (count--) {
        *d++ = val;
    }

    return dest;
}

void* Kstd::memcpy(void* dest, const void* src, uint64_t size) {
    uint8_t* d = static_cast<uint8_t*>(dest);
    const uint8_t* s = static_cast<const uint8_t*>(src);

    uint64_t* d64 = reinterpret_cast<uint64_t*>(d);
    const uint64_t* s64 = reinterpret_cast<const uint64_t*>(s);

    while (size >= 8) {
        *d64++ = *s64++;
        size -= 8;
    }

    d = reinterpret_cast<uint8_t*>(d64);
    s = reinterpret_cast<const uint8_t*>(s64);

    while (size--) {
        *d++ = *s++;
    }

    return dest;
}

int Kstd::memcmp(const void* ptr1, const void* ptr2, uint64_t size) {
    const uint8_t* p1 = static_cast<const uint8_t*>(ptr1);
    const uint8_t* p2 = static_cast<const uint8_t*>(ptr2);

    for (uint64_t i = 0; i < size; i++) {
        if (p1[i] != p2[i])
            return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}

int Kstd::strcmp(const char* str1, const char* str2) {
    const uint8_t* s1 = reinterpret_cast<const uint8_t*>(str1);
    const uint8_t* s2 = reinterpret_cast<const uint8_t*>(str2);

    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

uint64_t Kstd::strToUint64(const char* str, int base) {
    uint64_t sum = 0;
    const char* s = str;

    while (*s != '\0' && *s != ' ') {
        int digit = -1;
        if (*s >= '0' && *s <= '9')
            digit = *s - '0';
        else if (*s >= 'a' && *s <= 'z')
            digit = *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'Z')
            digit = *s - 'A' + 10;
        
        if (digit < 0 || digit >= base)
            break;
        
        sum = (sum * base) + digit;
        s++;
    }

    return sum;
}

extern volatile uint32_t* framebuffer_address;
extern uint64_t pitch;

void Kstd::print(const char* c) {
    Psf1Parser parser(Ramdisk::readFile("fonts/iso09.f16.psf"));
    const uint8_t* glyphData = parser.getGlyphData(c);

    for (uint8_t y = 0; y < parser.charsize; y++) {
        uint8_t row = glyphData[y];

        for (uint8_t x = 0; x < 8; x++) {
            if (row & (0x80 >> x)) {
                for (uint8_t sy = 0; sy < 4; sy++) {
                    for (uint8_t sx = 0; sx < 4; sx++) {
                        uint8_t posX = x * 4 + sx;
                        uint8_t posY = y * 4 + sy;
                        framebuffer_address[posX + posY * (pitch / 4)] = 0x00FF00FF;
                    }
                }
            }
        }
    }
}

extern "C" {
    void* memset(void* dest, int c, uint64_t count) {
        return Kstd::memset(dest, c, count);
    }

    void* memcpy(void* dest, const void* src, uint64_t size) {
        return Kstd::memcpy(dest, src, size);
    }
}