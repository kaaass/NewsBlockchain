#include "Int.h"

UInt32::UInt32(unsigned int val) {
    uintToBytes(val, bytes);
}

UInt32::operator unsigned int() {
    return byteToUint(bytes);
}

UInt32 &UInt32::operator=(unsigned int val) {
    uintToBytes(val, bytes);
    return *this;
}

std::vector<Byte> UInt32::toVector() {
    return std::vector<Byte>(bytes, bytes + 4);
}

void UInt32::uintToBytes(unsigned int uintVal, Byte (&bytes)[4]) {
    for (int i = 0; i < 4; i++) {
        bytes[3 - i] = static_cast<unsigned char>(uintVal);
        uintVal >>= (unsigned) 8;
    }
}

unsigned int UInt32::byteToUint(const Byte (&bytes)[4]) {
    unsigned int ret = 0;
    for (int i = 0; i < 4; i++) {
        ret |= static_cast<unsigned int>(bytes[3 - i] << 8 * i);
    }
    return ret;
}

void UInt16::uintToBytes(unsigned short uintVal, Byte (&bytes)[2]) {
    for (int i = 0; i < 2; i++) {
        bytes[1 - i] = static_cast<unsigned char>(uintVal);
        uintVal >>= (unsigned) 8;
    }
}

unsigned short UInt16::byteToUint(const Byte (&bytes)[2]) {
    unsigned short ret = 0;
    for (int i = 0; i < 2; i++) {
        ret |= static_cast<unsigned short>(bytes[1 - i] << 8 * i);
    }
    return ret;
}

UInt16::UInt16(unsigned short val) {
    uintToBytes(val, bytes);
}

UInt16::operator unsigned short() {
    return byteToUint(bytes);
}

UInt16 &UInt16::operator=(unsigned short val) {
    uintToBytes(val, bytes);
    return *this;
}

std::vector<Byte> UInt16::toVector() {
    return std::vector<Byte>(bytes, bytes + 2);
}