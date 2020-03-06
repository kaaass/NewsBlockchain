#include <cassert>
#include <huffman/Huffman.h>
#include <util/StringUtil.h>
#include <ctime>
#include <cmath>
#include <hash/Hash.h>

#include "ChainBlock.h"

ChainBlock::ChainBlock(const std::string &data, UInt32 prevHash, UInt blockId) {
    // 分段
    auto paras = StringUtil::splitParagraph(data);
    // 压缩
    auto compResult = Huffman::compress(paras);
    auto &dict = compResult.dictionary;
    auto &compData = compResult.data;
    // 写区块体
    auto &bodyData = blockBody.dataBlocks;
    bodyData.push_back(dict);
    bodyData.insert(bodyData.end(), compData.begin(), compData.end());
    // 建哈希树
    buildHashTree();
    blockBody.blockHashOffset = calcBlockHashOffset(bodyData.size());
    // 写区块头
    blockHeader.blockId = blockId;
    blockHeader.size = bodyData.size();
    blockHeader.hashRoot = blockBody.hashTree[0];
    blockHeader.prevBlockHash = prevHash;
    blockHeader.timestamp = time(nullptr);
}

void ChainBlock::buildHashTree() {
    auto &hashTree = blockBody.hashTree;
    auto &bodyData = blockBody.dataBlocks;
    ByteBuffer buf;

    ULong rest = bodyData.size();

    // 先计算一遍哈希
    for (auto &buffer : bodyData) {
        hashTree.push_back(Hash::run(buffer));
    }

    // 之后开始构建哈希树结构
    while (rest > 1) {
        ULong destSize, nextRest = rest;

        // 计算剩余
        if (nextRest % 2u == 1u)
            nextRest++;
        nextRest = nextRest >> 1u;
        // 计算目标大小
        destSize = IntUtil::next64Pow2(nextRest);

        std::vector<UInt32> temp(destSize);

        // rest/2 两两计算哈希
        for (ULong i = 0; i < rest / 2; i++) {
            auto &hashLf = hashTree[2 * i];
            auto &hashRt = hashTree[2 * i + 1];
            buf.clear();
            buf.push_back(&hashLf, sizeof(UInt32));
            buf.push_back(&hashRt, sizeof(UInt32));
            temp[i] = Hash::run(buf);
        }

        // 有多余则与0x00000000计算哈希
        if (rest % 2u == 1u) {
            auto &hashLf = hashTree[rest - 1];
            buf.clear();
            buf.push_back(&hashLf, sizeof(UInt32));
            buf.push_back(0u).push_back(0u)
                    .push_back(0u).push_back(0u);
            temp[rest / 2] = Hash::run(buf);
        }

        rest = nextRest;
        hashTree.insert(hashTree.begin(), temp.begin(), temp.end());
    }
}

ChainBlock::HashTreeIndex ChainBlock::calcBlockHashOffset(ChainBlock::DataBlockIndex size) {
    return IntUtil::next64Pow2(size) - 1;
}

UInt32 ChainBlock::getBlockHash() const {
    ByteBuffer buf;
    writeBuffer(buf);
    return Hash::run(buf);
}

const ChainBlock::Header &ChainBlock::getHeader() const {
    return blockHeader;
}

const std::vector<UInt32> &ChainBlock::getHashTree() const {
    return blockBody.hashTree;
}

const std::vector<ByteBuffer> &ChainBlock::getAllDataBlock() const {
    return blockBody.dataBlocks;
}

ByteBuffer &ChainBlock::operator[](ChainBlock::DataBlockIndex ind) {
    assert(0 <= ind && ind < size());
    return blockBody.dataBlocks[ind];
}

const ByteBuffer &ChainBlock::operator[](ChainBlock::DataBlockIndex ind) const {
    assert(0 <= ind && ind < size());
    return blockBody.dataBlocks[ind];
}

ByteBuffer ChainBlock::getDecomposedDataBlock(ChainBlock::DataBlockIndex ind) const {
    assert(0 <= ind && ind < size());
    if (ind == 0)
        return this->operator[](0);
    // 解压
    auto dict = getDictBlock();
    return Huffman::decompress(dict, this->operator[](ind));
}

std::vector<ByteBuffer> ChainBlock::getAllDecomposedDataBlock() const {
    std::vector<ByteBuffer> ret;
    for (ULong ind = 1; ind < size(); ind++) {
        ret.push_back(getDecomposedDataBlock(ind));
    }
    return ret;
}

UInt32 ChainBlock::getDataBlockHashById(ChainBlock::DataBlockIndex ind) const {
    auto hashInd = getHashTreeIndexById(ind);
    assert(hashInd >= 0 && (size_t) hashInd < getHashTree().size());
    return getHashTree()[hashInd];
}

ChainBlock::HashTreeIndex ChainBlock::getHashTreeIndexById(ChainBlock::DataBlockIndex ind) const {
    assert(0 <= ind && ind < size());
    return blockBody.blockHashOffset + ind;
}

const ByteBuffer &ChainBlock::getDictBlock() const {
    return blockBody.dataBlocks[0];
}

ChainBlock::DataBlockIndex ChainBlock::size() const {
    return blockHeader.size;
}

ByteBuffer &ChainBlock::writeBuffer(ByteBuffer &buffer) const {
    /*
     * 区块头部
     */
    // 4字节 区块ID
    buffer.write(blockHeader.blockId);
    // 4字节 前一区块的哈希值
    buffer.write(blockHeader.prevBlockHash);
    // 4字节 哈希树树根
    buffer.write(blockHeader.hashRoot);
    // 8字节 建块时间戳
    buffer.write(blockHeader.timestamp);
    // 8字节 数据块数量 block_size
    buffer.write(blockHeader.size);

    /*
     * 区块体
     */
    // 8字节              哈希树长度（不包含树根） hash_size
    buffer.write((ULong) blockBody.hashTree.size() - 1);
    // 4 * hash_size字节  哈希树（不包含树根）
    buffer.push_back(blockBody.hashTree.data() + 1, sizeof(UInt32) * (blockBody.hashTree.size() - 1));
    // 每块数据块
    for (auto &data : blockBody.dataBlocks) {
        // 4字节      数据块内容长度 length
        buffer.write((UInt) data.size());
        // length字节 数据块内容
        buffer.push_back(data.data(), data.size());
    }
    return buffer;
}

bool ChainBlock::isHashTreePadding(ChainBlock::HashTreeIndex ind) const {
    if ((size_t) ind < getHashTree().size())
        return isHashTreePadding(size(), ind);
    else
        return false; // 超界
}

bool ChainBlock::isHashTreePadding(ChainBlock::DataBlockIndex size, ChainBlock::HashTreeIndex ind) {
    auto offset = IntUtil::next64Pow2(size) - 1;
    // 不是最后一层递归处理
    if (ind < offset) {
        return isHashTreePadding(size, HTREE_LF(ind))
               && isHashTreePadding(size, HTREE_RT(ind));
    }
    return ind >= offset + size;
}
