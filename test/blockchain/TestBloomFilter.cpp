#include <gtest/gtest.h>

#include <blockchain/BloomFilter.h>

TEST(TestBloomFilter, testRun) {

    BloomFilter bf(4);

    bf.insert(ByteBuffer::str("Gray"));
    bf.insert(ByteBuffer::str("Rave"));
    bf.insert(ByteBuffer::str("Crave"));
    bf.insert(ByteBuffer::str("Deprave"));

    ASSERT_TRUE(bf.contain(ByteBuffer::str("Gray")));
    ASSERT_TRUE(bf.contain(ByteBuffer::str("Rave")));
    ASSERT_TRUE(bf.contain(ByteBuffer::str("Crave")));
    ASSERT_TRUE(bf.contain(ByteBuffer::str("Deprave")));

    ASSERT_FALSE(bf.contain(ByteBuffer::str("Rhongomyniad")));
}

TEST(TestBloomFilter, testSerialize) {

    BloomFilter bf(4);

    bf.insert(ByteBuffer::str("Gray"));
    bf.insert(ByteBuffer::str("Rave"));
    bf.insert(ByteBuffer::str("Crave"));
    bf.insert(ByteBuffer::str("Deprave"));

    ByteBuffer temp;
    bf.writeToBuffer(temp);

    BloomFilter *bfPtr = BloomFilter::readFromBuffer(temp);

    ASSERT_TRUE(bfPtr->contain(ByteBuffer::str("Gray")));
    ASSERT_TRUE(bfPtr->contain(ByteBuffer::str("Rave")));
    ASSERT_TRUE(bfPtr->contain(ByteBuffer::str("Crave")));
    ASSERT_TRUE(bfPtr->contain(ByteBuffer::str("Deprave")));

    ASSERT_FALSE(bfPtr->contain(ByteBuffer::str("Rhongomyniad")));
}