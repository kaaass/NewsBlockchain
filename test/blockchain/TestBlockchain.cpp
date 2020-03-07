#include <gtest/gtest.h>

#include <blockchain/Blockchain.h>

TEST(TestBlockchain, testCreate) {
    ByteBuffer expected;
    ByteBuffer actual;

    Blockchain::clear();

    Blockchain::create("123123");
    Blockchain::create("abc\ncba\nabc");

    ASSERT_EQ(2u, Blockchain::size());

    auto &block0 = Blockchain::get(0);

    Blockchain::GLOBAL_CHAIN[0].get().writeBuffer(expected);
    block0.writeBuffer(actual);
    ASSERT_EQ(expected, actual);
    ASSERT_EQ(0u, (UInt) block0.getHeader().blockId);
    ASSERT_EQ(UInt32(0), block0.getHeader().prevBlockHash);
    ASSERT_EQ(ByteBuffer::str("123123"), block0.getDecomposedDataBlock(1));

    auto &block1 = Blockchain::get(1);
    expected.clear();
    actual.clear();
    Blockchain::GLOBAL_CHAIN[1].get().writeBuffer(expected);
    block1.writeBuffer(actual);
    ASSERT_EQ(expected, actual);
    ASSERT_EQ(1u, (UInt) block1.getHeader().blockId);
    ASSERT_EQ(block0.getBlockHash(), block1.getHeader().prevBlockHash);
    ASSERT_EQ(ByteBuffer::str("abc"), block1.getDecomposedDataBlock(1));
    ASSERT_EQ(ByteBuffer::str("cba"), block1.getDecomposedDataBlock(2));
    ASSERT_EQ(ByteBuffer::str("abc"), block1.getDecomposedDataBlock(3));

    // getGlobalChain
    auto chain = Blockchain::getGlobalChain();
    ASSERT_EQ(Blockchain::GLOBAL_CHAIN.size(), chain.size());
    ASSERT_EQ(ByteBuffer::str("123123"), chain[0].get().getDecomposedDataBlock(1));
    ASSERT_EQ(ByteBuffer::str("cba"), chain[1].get().getDecomposedDataBlock(2));

    Blockchain::clear();
}

TEST(TestBlockchain, testCheck)
{
    Blockchain::clear();

    Blockchain::create("123123");
    ASSERT_TRUE(Blockchain::check());
    Blockchain::create("abc\ncba\nabc");
    ASSERT_TRUE(Blockchain::check());
    Blockchain::create("abc\n c b a\na bc12 3");
    ASSERT_TRUE(Blockchain::check());
}