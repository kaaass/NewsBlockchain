#include "Byte.h"
#include <cassert>

Bit::operator unsigned char() {
    return b;
}

Bit Byte::BitPattern::operator[](int ind) const {
    assert(ind >= 0 && ind <= 7);
#ifndef MEM_LITLLE_ENDIAN
    ind = 7 - ind;
#endif
    switch (ind) {
        case 0:
            return b0;
        case 1:
            return b1;
        case 2:
            return b2;
        case 3:
            return b3;
        case 4:
            return b4;
        case 5:
            return b5;
        case 6:
            return b6;
        case 7:
            return b7;
        default:
            return b0;
    }
}

Bit Byte::operator[](int ind) const {
    return bits[ind];
}

std::ostream &operator<<(std::ostream &out, const Byte &byte) {
    for (int i = 7; i >= 0; i--) {
        out << byte.bits[i];
    }
    return out;
}

Byte::operator unsigned char() const {
    return val;
}

std::ostream &operator<<(std::ostream &out, const Bit &bit) {
    out << (bit.b != 0);
    return out;
}