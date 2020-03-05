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

    ULong rest = bodyData.size();

    // 先计算一遍哈希
    for (auto buffer : bodyData) {
        hashTree.push_back(Hash::run(buffer));
    }

    // 之后开始构建哈希树结构
    bool leaveLayer = true;
    while (rest > 0) {
        ULong destSize;

        // 计算目标大小
        if (leaveLayer) {
            // 叶子层无需填充
            destSize = rest;
            leaveLayer = false;
        } else {
            // 其它层填充至2次幂
            destSize = IntUtil::next64Pow2(rest);
        }

        std::vector<UInt32> temp(destSize);

        // rest/2 两两计算哈希
        for (ULong i = 0; i < rest / 2; i++) {
            auto &hashLf = hashTree[2 * i];
            auto &hashRt = hashTree[2 * i + 1];
            ByteBuffer buf;
            buf.push_back(&hashLf, sizeof(UInt32));
            buf.push_back(&hashRt, sizeof(UInt32));
            temp[i] = Hash::run(buf);
        }

        // 有多余则与0x00000000计算哈希
        if (rest % 2u >= 0) {
            auto &hashLf = hashTree[rest - 1];
            ByteBuffer buf;
            buf.push_back(&hashLf, sizeof(UInt32));
            buf.push_back(0u).push_back(0u)
                    .push_back(0u).push_back(0u);
            temp[rest / 2] = Hash::run(buf);
            // 计算方便
            rest++;
        }

        rest = rest >> 1u;
        hashTree.insert(hashTree.begin(), temp.begin(), temp.end());
    }
}

ChainBlock::HashTreeIndex ChainBlock::calcBlockHashOffset(ChainBlock::DataBlockIndex size) {
    ULong layer = log2(size + (double) 0.5);
    return (1 + layer) * layer / 2;
}

UInt32 ChainBlock::getBlockHash() const {
    // TODO
    return UInt32();
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
