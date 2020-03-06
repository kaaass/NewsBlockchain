#ifndef NEWS_BLOCKCHAIN_BLOCKCHAIN_H
#define NEWS_BLOCKCHAIN_BLOCKCHAIN_H

#include <util/Types.h>
#include <vector>
#include <string>
#include <blockchain/ChainBlock.h>

#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

/**
 * 区块链管理类
 */
class Blockchain {

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

#ifdef UNIT_TEST

    /**
     * 清空区块链
     *
     * 仅在单元测试中可见。
     * @return
     */
    static void clear();

    FRIEND_TEST(TestBlockchain, testCreate);

#endif
};

#endif //NEWS_BLOCKCHAIN_BLOCKCHAIN_H
