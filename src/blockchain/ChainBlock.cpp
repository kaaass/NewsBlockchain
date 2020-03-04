#include <cassert>
#include <huffman/Huffman.h>
#include "ChainBlock.h"

ChainBlock::ChainBlock(std::string data) {
    // TODO
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
