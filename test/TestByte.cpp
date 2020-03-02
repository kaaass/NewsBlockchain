#include <gtest/gtest.h>

#include <util/Byte.h>

TEST(TestByte, testCast) {
    Byte byte = 0b10010010u;
    ASSERT_EQ(0b10010010u, (unsigned char) byte);
}

TEST(TestByte, testIndex) {
    Byte byte = 0b10100110u;
    ASSERT_EQ(1u, (unsigned char) byte[0]);
    ASSERT_EQ(0u, (unsigned char) byte[1]);
    ASSERT_EQ(1u, (unsigned char) byte[2]);
    ASSERT_EQ(0u, (unsigned char) byte[3]);
    ASSERT_EQ(0u, (unsigned char) byte[4]);
    ASSERT_EQ(1u, (unsigned char) byte[5]);
    ASSERT_EQ(1u, (unsigned char) byte[6]);
    ASSERT_EQ(0u, (unsigned char) byte[7]);
}