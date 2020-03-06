#include <gtest/gtest.h>

#include <blockchain/ChainBlock.h>
#include <huffman/Huffman.h>
#include <util/StringUtil.h>
#include <hash/Hash.h>

TEST(TestChainBlock, testConstruct) {
    std::string data = "123\n345";
    UInt32 prevHash = 0x12344321u;

    ChainBlock block(data, prevHash, 1u);

    // 区块头
    // Hash 相关单独测试
    auto &header = block.getHeader();
    ASSERT_EQ(1u, header.blockId);
    ASSERT_EQ(prevHash, header.prevBlockHash);
    ASSERT_GE(5, time(nullptr) - header.timestamp);
    ASSERT_EQ(3u, block.size());

    // 区块体
    // 详细检查单独测试

    // 0:   x
    // 1: x   x
    // 3:b b b _
    ASSERT_EQ(3u, block.blockBody.blockHashOffset);
    ASSERT_EQ(6u, block.blockBody.hashTree.size());
    ASSERT_EQ(3u, block.blockBody.dataBlocks.size());

    // 区块体内容
    auto result = Huffman::compress(StringUtil::splitParagraph(data));
    ASSERT_EQ(result.dictionary, block.getDictBlock());
    ASSERT_EQ(result.data[0], block[1]);
    ASSERT_EQ(result.data[1], block[2]);

    // const取下标
    const auto &cBlock = block;
    ASSERT_EQ(block[1], cBlock[1]);
    ASSERT_EQ(block[2], cBlock[2]);
}

UInt32 hashFor(UInt32 a, UInt32 b) {
    ByteBuffer buf;
    buf.push_back(&a, sizeof(UInt32));
    buf.push_back(&b, sizeof(UInt32));
    return Hash::run(buf);
}

TEST(TestChainBlock, testHashTree1) {
    // 树形：
    // 0:   x
    // 1: x   x
    // 3:b b b b

    std::string data = "abc\ndef\nghi";
    ChainBlock block(data, 0x0u, 1u);
    auto &hashTree = block.blockBody.hashTree;
    auto &dataBlocks = block.blockBody.dataBlocks;
    auto &offset = block.blockBody.blockHashOffset;

    ASSERT_EQ(7u, hashTree.size());
    ASSERT_EQ(3u, block.blockBody.blockHashOffset);
    // 叶子节点
    for (ULong i = 0; i < dataBlocks.size(); i++) {
        ASSERT_EQ(Hash::run(dataBlocks[i]), hashTree[offset + i]);
        ASSERT_EQ(Hash::run(dataBlocks[i]), block.getDataBlockHashById(i));
    }
    // 其他节点
    ASSERT_EQ(hashFor(hashTree[3], hashTree[4]),
              hashTree[1]);
    ASSERT_EQ(hashFor(hashTree[5], hashTree[6]),
              hashTree[2]);
    ASSERT_EQ(hashFor(hashTree[1], hashTree[2]),
              hashTree[0]);
}

TEST(TestChainBlock, testHashTree2) {
    // 树形：
    // 0:       x
    // 1:   x       x
    // 3: x   x   x  _
    // 7:b b b b b

    std::string data = "abc\ndef\nghi\njkl";
    ChainBlock block(data, 0x0u, 1u);
    auto &hashTree = block.getHashTree();
    auto &dataBlocks = block.getAllDataBlock();
    auto &offset = block.blockBody.blockHashOffset;

    ASSERT_EQ(12u, hashTree.size());
    ASSERT_EQ(7u, block.blockBody.blockHashOffset);
    // 叶子节点
    for (ULong i = 0; i < dataBlocks.size(); i++) {
        ASSERT_EQ(Hash::run(dataBlocks[i]), hashTree[offset + i]);
        ASSERT_EQ(Hash::run(dataBlocks[i]), block.getDataBlockHashById(i));
    }
    // 其他节点
    for (ULong i = 0; i < offset; i++) {
        std::cout << "Test for: " << i << std::endl;
        if (block.isHashTreePadding(i)) {
            // 空白
            ASSERT_EQ(UInt32(0x0u), hashTree[i]);
            continue;
        }
        auto rtIndex = HTREE_RT(i);
        UInt32 rt = rtIndex >= hashTree.size()
                    ? UInt32(0)
                    : hashTree[rtIndex];
        ASSERT_EQ(hashFor(hashTree[HTREE_LF(i)], rt),
                  hashTree[i]);
    }
    // 空白
    ASSERT_EQ(UInt32(0x0u), hashTree[6]);
}

TEST(TestChainBlock, testHashTreeIndexCalc) {
    // calcBlockHashOffset

    // 树形：
    // 0:  x
    // 1: b b
    ASSERT_EQ(1u, ChainBlock::calcBlockHashOffset(2));

    // 树形：
    // 0:               x
    // 1:       x              x
    // 3:   x       x       x    _
    // 7: x   x   x   x   x  _ _   _
    //15:b b b b b b b b b
    ASSERT_EQ(15u, ChainBlock::calcBlockHashOffset(9));

    // isHashTreePadding

    // 树形：
    // 0:               x
    // 1:       x              x
    // 3:   x       x       x    _
    // 7: x   x   x   x   x  _ _   _
    //15:b b b b b b b b b
    ChainBlock block("1\n2\n3\n4\n5\n6\n7\n8\n9", 0x0u, 1u);
    for (ULong i = 0; i < 6; i++)
        ASSERT_FALSE(block.isHashTreePadding(i));
    ASSERT_TRUE(block.isHashTreePadding(6));
    for (ULong i = 7; i < 7 + 5; i++)
        ASSERT_FALSE(block.isHashTreePadding(i));
    ASSERT_TRUE(block.isHashTreePadding(12));
    ASSERT_TRUE(block.isHashTreePadding(13));
    ASSERT_TRUE(block.isHashTreePadding(14));
    for (ULong i = 15; i < 15 + 16; i++)
        ASSERT_FALSE(block.isHashTreePadding(i));
}

TEST(TestChainBlock, testDecompose) {
    std::string data = "akakasd\n125s1\n 31 4a ";
    UInt32 prevHash = 0x12344321u;

    ChainBlock block(data, prevHash, 1u);

    // getAllDecomposedDataBlock
    auto decomp = block.getAllDecomposedDataBlock();
    ASSERT_EQ(ByteBuffer::str("akakasd"), decomp[0]);
    ASSERT_EQ(ByteBuffer::str("125s1"), decomp[1]);
    ASSERT_EQ(ByteBuffer::str(" 31 4a "), decomp[2]);
    // getDecomposedDataBlock
    ASSERT_EQ(ByteBuffer::str(" 31 4a "), block.getDecomposedDataBlock(3));
}

TEST(TestChainBlock, testWriteBuffer) {
    ByteBuffer expected;

    std::string data = "123";
    UInt32 prevHash = 0x12344321u;

    ChainBlock block(data, prevHash, 1u);

    /*
     * 拼接正确的ByteBuffer
     */
    // 压缩
    auto result = Huffman::compress(StringUtil::splitParagraph(data));
    auto dataBlocks = std::vector<ByteBuffer>{result.dictionary, result.data[0]};
    auto hashLf = Hash::run(dataBlocks[0]);
    auto hashRt = Hash::run(dataBlocks[1]);
    auto hashRoot = hashFor(hashLf, hashRt);
    // 区块头
    // 4字节 区块ID
    expected.write(1u);
    // 4字节 前一区块的哈希值
    expected.write(prevHash);
    // 4字节 哈希树树根
    expected.write(hashRoot);
    // 8字节 建块时间戳
    expected.write(block.getHeader().timestamp);
    // 8字节 数据块数量 block_size
    expected.write((ULong) 2u);
    // 区块体
    // 8字节              哈希树长度（不包含树根） hash_size
    expected.write((ULong) 2u);
    // 4 * hash_size字节  哈希树（不包含树根）
    expected.write(hashLf);
    expected.write(hashRt);
    // 每块数据块：
    //  4字节      数据块内容长度 length
    //  length字节 数据块内容
    expected.write((UInt) dataBlocks[0].size());
    expected = expected + dataBlocks[0];
    expected.write((UInt) dataBlocks[1].size());
    expected = expected + dataBlocks[1];

    /*
     * 校验
     */
    ByteBuffer actual;
    block.writeBuffer(actual);
    ASSERT_EQ(expected, actual);
}

TEST(TestChainBlock, testBlockHash) {
    std::string data = "123\n3425\naasdasd";
    UInt32 prevHash = 0x12344321u;

    ChainBlock block(data, prevHash, 1u);
    ByteBuffer buffer;
    block.writeBuffer(buffer);
    ASSERT_EQ(Hash::run(buffer), block.getBlockHash());
}