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

TEST(TestBlockchain, testCheck) {
    Blockchain::clear();

    Blockchain::create("123123");
    ASSERT_TRUE(Blockchain::check());
    Blockchain::create("abc\ncba\nabc");
    ASSERT_TRUE(Blockchain::check());
    Blockchain::create("abc\n c b a\na bc12 3");
    ASSERT_TRUE(Blockchain::check());
    // 篡改第一块
    auto &block0 = Blockchain::GLOBAL_CHAIN[0].get();
    ChainBlock origin = block0;
    block0[1].push_back(0x12u);
    ASSERT_FALSE(Blockchain::check());
    block0 = origin;
    ASSERT_TRUE(Blockchain::check());
    // 篡改第二块
    auto &block1 = Blockchain::GLOBAL_CHAIN[1].get();
    origin = block1;
    block1[2].push_back(0x34u);
    ASSERT_FALSE(Blockchain::check());
    block1 = origin;
    ASSERT_TRUE(Blockchain::check());
    // 篡改第三块
    auto &block2 = Blockchain::GLOBAL_CHAIN[2].get();
    origin = block2;
    block2[3] = ByteBuffer::str("2333");
    ASSERT_FALSE(Blockchain::check());
    block2 = origin;
    ASSERT_TRUE(Blockchain::check());
}

TEST(TestBlockchain, testValidateNews) {
    Blockchain::clear();

    Blockchain::create("123123");
    Blockchain::create("abc\ncba\nabc");
    Blockchain::create("abc\n c b a\na bc12 3");

    //完全相同
    std::string test0 = "123123";
    std::vector<UInt> vec0;
    ASSERT_EQ(vec0, Blockchain::validateNews(test0, 0));
    //多加/n
    std::string test01 = "\n\n123123\n\n";
    std::vector<UInt> vec01;
    ASSERT_EQ(vec01, Blockchain::validateNews(test01, 0));
    //段落数和真实新闻相同
    std::string test1 = "acb\nbca\nabc";
    std::vector<UInt> vec1{1, 2};
    ASSERT_EQ(vec1, Blockchain::validateNews(test1, 1));
    std::string test2 = "ab\ncb\nab";
    std::vector<UInt> vec2{1, 2, 3};
    ASSERT_EQ(vec2, Blockchain::validateNews(test2, 1));
    //段落比真实新闻少
    std::string test3 = "abc\ncb";
    std::vector<UInt> vec3{2, 3};
    ASSERT_EQ(vec3, Blockchain::validateNews(test3, 1));
    std::string test4 = "abc\ncba\n";
    std::vector<UInt> vec4 = {3};
    ASSERT_EQ(vec4, Blockchain::validateNews(test4, 1));
    //段落比真实新闻多
    std::string test5 = "abc\n c b a\na bc12 3\nav\n\n";
    std::vector<UInt> vec5{4};
    ASSERT_EQ(vec5, Blockchain::validateNews(test5, 2));
}