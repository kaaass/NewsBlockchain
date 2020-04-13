#ifndef NEWS_BLOCKCHAIN_BLOCKCHAIN_H
#define NEWS_BLOCKCHAIN_BLOCKCHAIN_H

#include <util/Types.h>
#include <blockchain/ChainBlock.h>
#include <vector>
#include <string>
#include <functional>
#include <util/StringUtil.h>
#include <huffman/Huffman.h>
#include <hash/Hash.h>

#ifdef UNIT_TEST

#include <gtest/gtest.h>

#endif

/**
 * 区块链管理类
 */
class Blockchain {

    enum EditOperation {
        Copy,
        Add,
        Remove,
        Replace
    };


    /**
     * 保存所有区块链的引用
     */
    static std::vector<std::reference_wrapper<ChainBlock>> GLOBAL_CHAIN;

public:

    /**
     * 创建区块至区块链
     * @param data
     * @return 该区块的ID
     */
    static UInt create(const std::string &data);

    /**
     * 通过区块ID获得一个区块
     * @param blockId
     * @return
     */
    static const ChainBlock &get(UInt blockId);

    /**
     * 当前区块链大小
     * @return
     */
    static size_t size();

    /**
     * 获得所有区块的引用
     * @return
     */
    static std::vector<std::reference_wrapper<const ChainBlock>> getGlobalChain();

    /**
     *  检测区块头中包含的前一区块的哈希值 和 真实的前一区块的哈希值 是否相同
     *  @param
     *  @return
     */
    static bool check();

    /**
     * 判断给定新闻是否篡改过
     * @param data
     * @param blockId
     * @return 获取篡改新闻所在自然段
     */
    static std::vector<UInt32> validateNews(const std::string &data, UInt blockId);

#ifdef UNIT_TEST

    /**
     * 清空区块链
     *
     * 仅在单元测试中可见。
     * @return
     */
    static void clear();

    FRIEND_TEST(TestBlockchain, testCreate);
    FRIEND_TEST(TestBlockchain, testCheck);

#endif
};

#endif //NEWS_BLOCKCHAIN_BLOCKCHAIN_H
