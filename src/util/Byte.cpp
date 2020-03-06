#include "Byte.h"
#include "Types.h"
#include <cassert>
#include <sstream>
#include <iomanip>

Bit::operator unsigned char() {
    return b;
}

Bit LittleEndianByte::BitPattern::operator[](int ind) const {
    assert(ind >= 0 && ind <= 7);
#ifdef MEM_BIT_BIG_ENDIAN
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

Bit LittleEndianByte::operator[](int ind) const {
    return bits[ind];
}

std::ostream &operator<<(std::ostream &out, const LittleEndianByte &byte) {
    for (int i = 7; i >= 0; i--) {
        out << byte.bits[i];
    }
    return out;
}

LittleEndianByte::operator unsigned char() const {
    return val;
}

std::string LittleEndianByte::toHexString() const {
    std::stringstream sstream;
    std::string ret;
    sstream << std::setw(2) << std::setfill('0') << std::hex << (UInt) val;
    sstream >> ret;
    return ret;
}

std::ostream &operator<<(std::ostream &out, const Bit &bit) {
    out << (bit.b != 0);
    return out;
}