#include <gtest/gtest.h>

#include <util/Int.h>

TEST(TestInt, testCast) {
    UInt32 uInt32 = 0x12345678u;
    ASSERT_EQ(0x12345678u, (unsigned int) uInt32);

    UInt16 uInt16 = 0x2434u;
    ASSERT_EQ(0x2434u, (unsigned int) uInt16);
}

TEST(TestInt, testToVector) {
    std::vector<Byte> vec;
    // 字节序为大端，即小index对应高位数据
    UInt32 uInt32 = 0x87654321u;
    vec = uInt32.toVector();
    ASSERT_EQ(0x87u, (unsigned int) vec[0]);
    ASSERT_EQ(0x65u, (unsigned int) vec[1]);
    ASSERT_EQ(0x43u, (unsigned int) vec[2]);
    ASSERT_EQ(0x21u, (unsigned int) vec[3]);

    UInt16 uInt16 = 0x6543u;
    vec = uInt16.toVector();
    ASSERT_EQ(0x65u, (unsigned int) vec[0]);
    ASSERT_EQ(0x43u, (unsigned int) vec[1]);
}