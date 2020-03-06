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
    ASSERT_EQ(1u, block.blockHeader.blockId);
    ASSERT_EQ(prevHash, block.blockHeader.prevBlockHash);
    ASSERT_GE(5, time(nullptr) - block.blockHeader.timestamp);
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
    ASSERT_EQ(result.dictionary, block[0]);
    ASSERT_EQ(result.data[0], block[1]);
    ASSERT_EQ(result.data[1], block[2]);
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
        if (block.isHashTreePadding(i))
            continue;
        std::cout << "Test for: " << i << std::endl;
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

    // TODO isHashTreePaddings
}

TEST(TestChainBlock, testDataBlock) {
    // TODO getDecomposedDataBlock
    // TODO getAllDecomposedDataBlock
}