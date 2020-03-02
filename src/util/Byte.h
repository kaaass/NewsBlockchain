#ifndef NEWS_BLOCKCHAIN_BYTE_H
#define NEWS_BLOCKCHAIN_BYTE_H

#include <iostream>

/**
 * 单比特
 */
struct Bit {
    unsigned char b : 1;

    Bit(unsigned char val) : b(val) {}

    operator unsigned char();

    friend std::ostream & operator<<(std::ostream &, const Bit&);
};

/**
 * 小端字节
 */
union LittleEndianByte {
    /**
     * 值
     */
    unsigned char val;

    /**
     * 包含比特
     */
    struct BitPattern {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;

        Bit operator[](int ind) const;
    } bits;

    LittleEndianByte() : val(0) {}

    LittleEndianByte(unsigned char val) : val(val) {}

    Bit operator[](int ind) const;

    operator unsigned char() const;

    friend std::ostream & operator<<(std::ostream &, const LittleEndianByte&);
};

/**
 * 单字节
 *
 * 比特序为小端，即低比特在低地址，高比特在高地址
 * 即大部分内存的存储格式
 */
typedef LittleEndianByte Byte;

#endif //NEWS_BLOCKCHAIN_BYTE_H
