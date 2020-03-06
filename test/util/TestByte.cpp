#include <gtest/gtest.h>

#include <util/Types.h>

TEST(TestByte, testCast) {
    Byte byte = 0b10010010u;
    ASSERT_EQ(0b10010010u, (UChar) byte);
}

TEST(TestByte, testIndex) {
    Byte byte = 0b10100110u;
    // 小端比特序
    ASSERT_EQ(0u, (UChar) byte[0]);
    ASSERT_EQ(1u, (UChar) byte[1]);
    ASSERT_EQ(1u, (UChar) byte[2]);
    ASSERT_EQ(0u, (UChar) byte[3]);
    ASSERT_EQ(0u, (UChar) byte[4]);
    ASSERT_EQ(1u, (UChar) byte[5]);
    ASSERT_EQ(0u, (UChar) byte[6]);
    ASSERT_EQ(1u, (UChar) byte[7]);
}

TEST(TestByte, testOutput) {
    std::stringstream sstream;
    std::string ret;

    // 测试输出
    Byte byte = 0b10011011u;
    sstream << byte;
    sstream >> ret;
    ASSERT_EQ("10011011", ret);

    // 测试十六进制字符串
    byte = 0x9fu;
    ASSERT_EQ("9f", byte.toHexString());

    byte = 0x0u;
    ASSERT_EQ("00", byte.toHexString());
}

TEST(TestByte, testEqual) {
    Byte byteA = 0b10010010u;
    Byte byteB = 0b10010010u;

    ASSERT_TRUE(byteA == byteB);
    ASSERT_TRUE((Byte) 0b11001100 == (Byte) 0b11001100);
}

TEST(TestByte, testOperate) {
    Byte byteA = 0b10010010u;

    ASSERT_EQ((UChar) 0b01001001u, byteA >> 1u);
    ASSERT_EQ((UChar) 0b00100100u, (UChar) (byteA << 1u));
    ASSERT_EQ((UChar) 0b10010011u, byteA + 1u);
    ASSERT_EQ((UChar) 0b10010001u, byteA - 1u);
}