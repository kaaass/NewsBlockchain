#include "Blockchain.h"
#include <cassert>

std::vector<std::reference_wrapper<ChainBlock>> Blockchain::GLOBAL_CHAIN;

UInt Blockchain::create(const std::string &data) {
    ChainBlock *newBlock = nullptr;
    UInt32 prevHash;
    UInt blockId = size();

    // 计算前一块哈希
    if (size() > 0) {
        prevHash = get(size() - 1).getBlockHash();
    } else {
        prevHash = 0u; // 第一块则为0
    }

    // 创建块
    newBlock = new ChainBlock(data, prevHash, blockId);
    // 加入链
    GLOBAL_CHAIN.push_back(std::ref(*newBlock));

    return blockId;
}

const ChainBlock &Blockchain::get(UInt blockId) {
    assert(blockId < size());
    return GLOBAL_CHAIN[blockId];
}

size_t Blockchain::size() {
    return GLOBAL_CHAIN.size();
}

std::vector<std::reference_wrapper<const ChainBlock>> Blockchain::getGlobalChain() {
    std::vector<std::reference_wrapper<const ChainBlock>> ret;
    ret.reserve(size());
    for (auto &ref: GLOBAL_CHAIN) {
        ret.emplace_back(ref.get());
    }
    return ret;
}

bool Blockchain::check() {
    UInt32 prevRealHash = 0u;  // 第一块为0
    UInt32 prevBlockHash; // 存储在区块中前一块的哈希值
    UInt32 blockID = 0u;

    for (auto iter = GLOBAL_CHAIN.cbegin(); iter != GLOBAL_CHAIN.cend(); iter++) {
        // 获取存储在区块中前一块的哈希值
        prevBlockHash = iter->get().getPrevBlockHash();
        // 计算真实的前一块哈希
        blockID = iter - GLOBAL_CHAIN.cbegin();
        if (blockID > 0) {
            prevRealHash = get(blockID - 1).getBlockHash();
        }
        if (prevBlockHash != prevRealHash) {
            return false;
        }
    }
    return true;
}

std::vector<UInt32> Blockchain::validateNews(const std::string &data, UInt blockId) {
    std::vector<ByteBuffer> bodyData; // 数据项
    std::vector<UInt32> wrongVec; // 数据项对应的hash
    UInt32 sectionNumber = 0u; // 段落序号
    auto paras = StringUtil::splitParagraph(data); // 分段
    const ChainBlock::DataBlockIndex newsSize = paras.size();
    const std::vector<UInt32> &hashTree = get(blockId).getHashTree(); // blockId对应的hashTree
    const ChainBlock::DataBlockIndex realSize = get(blockId).size() - 1; // 除去字典
    const std::vector<UInt32> hashVec(hashTree.cend() - realSize, hashTree.cend()); // blockId对应的数据项的hash
    auto compResult = Huffman::compress(paras); // 压缩
    auto &compData = compResult.data;
    bodyData.insert(bodyData.end(), compData.begin(), compData.end());

    for (sectionNumber = 0;
         sectionNumber < realSize && sectionNumber < newsSize; sectionNumber = sectionNumber + 1) { // 修改的段落
        UInt32 bufferHash = Hash::run(bodyData[sectionNumber]);
        if (bufferHash != hashVec[sectionNumber]) {
            wrongVec.emplace_back(sectionNumber + 1); // 自然段从1开始
        }
    }
    if (sectionNumber == newsSize) { // 缺少的段落
        while (sectionNumber < realSize) {
            wrongVec.emplace_back(sectionNumber + 1);
            return wrongVec;
        }
    }
    while (sectionNumber < newsSize) { // 增加的段落
        wrongVec.emplace_back(sectionNumber + 1);
        return wrongVec;
    }
}


#ifdef UNIT_TEST

void Blockchain::clear() {
    for (auto &ref: GLOBAL_CHAIN) {
        delete &ref.get();
    }
    GLOBAL_CHAIN.clear();
}

#endif