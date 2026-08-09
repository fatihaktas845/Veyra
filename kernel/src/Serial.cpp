#include "Serial.hpp"
#include "Io.hpp"

namespace Serial {
    uint16_t COM1_PORT = 0x3F8;
}

void Serial::write(const char* str) {
    const uint8_t* s = reinterpret_cast<const uint8_t*>(str);

    while (*s != '\0') {
        if (*s == '\n')
            Io::outb(COM1_PORT, '\r');
        
        Io::outb(COM1_PORT, *s);
        s++;
    }
}