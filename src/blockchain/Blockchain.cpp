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

#ifdef UNIT_TEST

void Blockchain::clear() {
    for (auto &ref: GLOBAL_CHAIN) {
        delete &ref.get();
    }
    GLOBAL_CHAIN.clear();
}

#endif