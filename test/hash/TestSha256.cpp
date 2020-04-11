#include <gtest/gtest.h>

#include <hash/Sha256.h>

TEST(TestSha256, testRun) {

    Sha256 sha256;

    ASSERT_EQ((UInt) 0xece8eb7bu,
            sha256.run(ByteBuffer::str("ILOVJLU")));
    ASSERT_EQ((UInt) 0xc7614b59,
              sha256.run(ByteBuffer::str("Data Structure")));
    ASSERT_EQ((UInt) 0xe3b0c442,
              sha256.run(ByteBuffer::str("")));
    ASSERT_EQ((UInt) 0x9fc57a7bu,
              sha256.run(ByteBuffer::str("I can eat glass, it doesn't hurt me.")));
}