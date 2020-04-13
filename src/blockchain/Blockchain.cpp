#include "Blockchain.h"
#include <cassert>
#include <algorithm>

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
        if (blockID == GLOBAL_CHAIN.size() - 1) { // 检测最后一个区块
            const std::vector<ByteBuffer> &allDataBlock = iter->get().getAllDataBlock();//所有数据块
            UInt32 sectionNumber = 0u; // 段落序号
            for (auto &ref: allDataBlock) {
                UInt32 checkHash = Hash::run(ref);
                UInt32 realHash = iter->get().getDataBlockHashById(sectionNumber);
                if (realHash != checkHash) {
                    return false;
                }
                sectionNumber = sectionNumber + 1;
            }
        }
    }
    return true;
}

std::vector<Blockchain::Difference> Blockchain::validateNews(const std::string &data, UInt blockId) {
    std::vector<ByteBuffer> bodyData; // 数据项
    auto paras = StringUtil::splitParagraph(data); // 分段
    const UInt32 fromLen = paras.size();
    const std::vector<ByteBuffer> &dataBlock = get(blockId).getAllDecomposedDataBlock();
    const std::vector<UInt32> &hashTree = get(blockId).getHashTree(); // blockId对应的hashTree
    const ChainBlock::DataBlockIndex realSize = get(blockId).size() - 1; // 除去字典
    const UInt32 toLen = get(blockId).size() - 1; // 除去字典
    std::vector<UInt32> distance((fromLen + 1) * (toLen + 1));
    std::vector<EditOperation> operations((fromLen + 1) * (toLen + 1));
    const std::vector<UInt32> hashVec(hashTree.cend() - realSize, hashTree.cend()); // blockId对应的数据项的hash
    auto compData = Huffman::compress(get(blockId)[0], paras); // 压缩
    bodyData.insert(bodyData.end(), compData.begin(), compData.end());
    auto index = [toLen](UInt32 i, UInt32 j) {
        return i * (toLen + 1) + j;
    };
    for (UInt32 i = 0; i <= fromLen; i = i + 1) {
        distance[index(i, 0)] = i;
        operations[index(i, 0)] = Remove;
    }
    for (UInt32 j = 0; j <= toLen; j = j + 1) {
        distance[index(0, j)] = j;
        operations[index(0, j)] = Add;
    }
    operations[0] = Copy;
    for (UInt32 i = 1; i <= fromLen; i = i + 1) {
        for (UInt32 j = 1; j <= toLen; j = j + 1) {
            auto ifAdd = distance[index(i, j - 1)] + 1;
            auto ifRemove = distance[index(i - 1, j)] + 1;
            UInt32 bufferHash = Hash::run(bodyData[i - 1]);
            bool needReplace = bufferHash != hashVec[j - 1];
            auto ifReplace = distance[index(i - 1, j - 1)] + (needReplace ? 1 : 0);
            if (ifAdd <= ifRemove && ifAdd <= ifReplace) {
                distance[index(i, j)] = ifAdd;
                operations[index(i, j)] = Add;
            } else if (ifRemove <= ifAdd && ifRemove <= ifReplace) {
                distance[index(i, j)] = ifRemove;
                operations[index(i, j)] = Remove;
            } else {
                distance[index(i, j)] = ifReplace;
                if (needReplace) {
                    operations[index(i, j)] = Replace;
                } else {
                    operations[index(i, j)] = Copy;
                }
            }
        }
    }
    std::vector<UInt32> result;
    auto i = fromLen, j = toLen;
    while (!(i == 0 && j == 0)) {
        EditOperation op = operations[index(i, j)];
        result.emplace_back(op);
        if (op == Add) {
            j = j - 1;
        } else if (op == Remove) {
            i = i - 1;
        } else {
            i = i - 1;
            j = j - 1;
        }
    }
    reverse(result.begin(), result.end());

    UInt32 fromIndex = 0u, toIndex = 0u;
    ByteBuffer correctContent, originContent;
    std::vector<Blockchain::Difference> differenceVec;
    for (auto operation : result) {
        std::cout << operation << std::endl;
        if (operation == Copy) {
            correctContent = paras[fromIndex];
            originContent = dataBlock[toIndex];
            differenceVec.emplace_back(Difference("Copy", originContent, correctContent));
            fromIndex = fromIndex + 1;
            toIndex = toIndex + 1;
        } else if (operation == Add) {
            originContent = dataBlock[toIndex];
            differenceVec.emplace_back("Remove", originContent);
            toIndex = toIndex + 1;
        } else if (operation == Remove) {
            correctContent = paras[fromIndex];
            differenceVec.emplace_back("Add", correctContent);
            fromIndex = fromIndex + 1;
        } else {
            correctContent = paras[fromIndex];
            originContent = dataBlock[toIndex];
            differenceVec.emplace_back("Replace", originContent, correctContent);
            fromIndex = fromIndex + 1;
            toIndex = toIndex + 1;
        }
    }
    return differenceVec;
}

#ifdef UNIT_TEST

void Blockchain::clear() {
    for (auto &ref: GLOBAL_CHAIN) {
        delete &ref.get();
    }
    GLOBAL_CHAIN.clear();
}

#endif