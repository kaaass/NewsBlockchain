#include <gtest/gtest.h>

#include <util/Types.h>

TEST(TestInt, testCast) {
    UInt32 uInt32 = 0x12345678u;
    ASSERT_EQ(0x12345678u, (UInt) uInt32);

    UInt16 uInt16 = 0x2434u;
    ASSERT_EQ(0x2434u, (UInt) uInt16);
}

TEST(TestInt, testToVector) {
    std::vector<Byte> vec;
    // 字节序为大端，即小index对应高位数据
    UInt32 uInt32 = 0x87654321u;
    vec = uInt32.toVector();
    ASSERT_EQ(0x87u, (UInt) vec[0]);
    ASSERT_EQ(0x65u, (UInt) vec[1]);
    ASSERT_EQ(0x43u, (UInt) vec[2]);
    ASSERT_EQ(0x21u, (UInt) vec[3]);

    UInt16 uInt16 = 0x6543u;
    vec = uInt16.toVector();
    ASSERT_EQ(0x65u, (UInt) vec[0]);
    ASSERT_EQ(0x43u, (UInt) vec[1]);
}

TEST(TestIntUtil, testNextPow2) {
    // 32-bit
    ASSERT_EQ((UInt) 1u, IntUtil::next32Pow2(1u));
    ASSERT_EQ((UInt) 4u, IntUtil::next32Pow2(3u));
    ASSERT_EQ((UInt) 8u, IntUtil::next32Pow2(8u));
    ASSERT_EQ((UInt) 128u, IntUtil::next32Pow2(123u));
    ASSERT_EQ(((UInt) 1u) << 14u, IntUtil::next32Pow2((1u << 13u) + 123u));
    ASSERT_EQ(((UInt) 1u) << 31u, IntUtil::next32Pow2((1u << 30u) + 123u));
    // 64-bit
    ASSERT_EQ((ULong) 1u, IntUtil::next64Pow2(1u));
    ASSERT_EQ((ULong) 4u, IntUtil::next64Pow2(3u));
    ASSERT_EQ((ULong) 8u, IntUtil::next64Pow2(8u));
    ASSERT_EQ((ULong) 128u, IntUtil::next64Pow2(123u));
    ASSERT_EQ((ULong) 256u, IntUtil::next64Pow2(153u));
    ASSERT_EQ(((ULong) 1u) << 14u, IntUtil::next64Pow2((1u << 13u) + 123u));
    ASSERT_EQ(((ULong) 1u) << 31u, IntUtil::next64Pow2((1u << 30u) + 123u));
    ASSERT_EQ(((ULong) 1u) << 50u, IntUtil::next64Pow2(((ULong) 1u << 49u) + 666u));
    ASSERT_EQ(((ULong) 1u) << 63u, IntUtil::next64Pow2(((ULong) 1u << 62u) + 12345u));
}