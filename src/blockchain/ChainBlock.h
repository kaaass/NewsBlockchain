#ifndef NEWS_BLOCKCHAIN_CHAINBLOCK_H
#define NEWS_BLOCKCHAIN_CHAINBLOCK_H

#include <util/Types.h>
#include <util/ByteBuffer.h>

/**
 * 64位时间戳
 */
typedef ULong Timestamp;

/**
 * 区块类
 */
class ChainBlock : public IByteBufferWriter {

    /*
     * 定义相关结构体
     */

    /**
     * 数据块下标
     *
     * 第0块数据块存放Huffman压缩的字典
     */
    typedef ULong DataBlockIndex;

    /**
     * 哈希树下标
     *
     * 平展哈希树数组的数组下标
     */
    typedef ULong HashTreeIndex;

    /**
     * 区块头部结构体
     *
     * 二进制格式（大端）：
     *  4字节 区块ID
     *  4字节 前一区块的哈希值
     *  4字节 哈希树树根
     *  8字节 建块时间戳
     *  8字节 数据块数量 block_size
     */
    struct Header {

        /**
         * 区块ID
         */
        UInt32 blockId;

        /**
         * 前一区块的哈希值
         *
         * 第一区块填充0x00
         */
        UInt32 prevBlockHash;

        /**
         * 哈希树树根
         */
        UInt32 hashRoot;

        /**
         * 建块时间戳
         */
        Timestamp timestamp;

        /**
         * 数据块数量
         */
        DataBlockIndex size;
    };

    /**
     * 区块体结构体
     *
     * 二进制格式（大端）：
     *  8字节              哈希树长度（不包含树根） hash_size
     *  4 * hash_size字节  哈希树（不包含树根）
     *  每块数据块：
     *    4字节      数据块内容长度 length
     *    length字节 数据块内容
     */
    struct Body {

        /**
         * 平展后的哈希树叶子结点（数据块）开始的偏移
         *
         * 注意哈希树有约定：所有叶子结点均在同一层
         */
        HashTreeIndex blockHashOffset;

        /**
         * 平展后的哈希树
         *
         * 存放了平展后的哈希树，并且包括哈希树根（但是计算块哈希值和存储时不会包含哈希树根）。因此树
         * 根的左右子节点分别为下标位置为0、1的哈希值。 某节点 i 的左右子节点可以通过 2*i 和 2*i+1
         * 计算，父节点可以使用 i / 2 计算。某数据块的哈希下标可以通过
         * ChainBlock::getHashTreeIndexById 方法计算。
         */
        std::vector<UInt32> hashTree;

        /**
         * 数据块
         */
        std::vector<ByteBuffer> dataBlocks;
    };

    /*
     * 定义字段与方法
     */

    /**
     * 区块头部
     */
    Header blockHeader;

    /**
     * 区块体
     */
    Body blockBody;

public:

    /**
     * 建立区块
     * @param data
     */
    ChainBlock(const std::string &data, UInt32 prevHash, UInt blockId);

    /**
     * 计算区块哈希
     * @return
     */
    UInt32 getBlockHash() const;

    /**
     * 获得区块头
     * @return
     */
    const Header &getHeader() const;

    /**
     * 获得平展哈希树
     * @return
     */
    const std::vector<UInt32> &getHashTree() const;

    /**
     * 获得数据块数量（包括字典）
     */
    DataBlockIndex size() const;

    /**
     * 获得所有数据块（包括字典）
     * @return
     */
    const std::vector<ByteBuffer> &getAllDataBlock() const;

    /**
     * 获得某个区块数据
     * @param ind 数据块id
     * @return
     */
    ByteBuffer &operator[](DataBlockIndex ind);

    const ByteBuffer &operator[](DataBlockIndex ind) const;

    /**
     * 获得某个数据块的解压数据
     * @param ind 数据块id，为0则返回原数据块内容
     * @return
     */
    ByteBuffer getDecomposedDataBlock(DataBlockIndex ind) const;

    /**
     * 获得所有解压后数据块
     * @return 不包含第0块（字典块）的数据块
     */
    std::vector<ByteBuffer> getAllDecomposedDataBlock() const;

    /**
     * 获得某数据块的哈希值
     * @param ind 数据块id
     * @return
     */
    UInt32 getDataBlockHashById(DataBlockIndex ind) const;

    /**
     * 获得某数据块对应的哈希树下标
     * @param ind 数据块id
     * @return
     */
    HashTreeIndex getHashTreeIndexById(DataBlockIndex ind) const;

    /**
     * 写区块数据为字节缓冲区
     *
     * 格式为：区块头 + 区块体。区块头、区块体格式参考对应结构体的注释。
     * @param buffer
     * @return
     */
    ByteBuffer &writeBuffer(ByteBuffer &buffer) const override;

private:

    /**
     * 获得字典块数据
     * @return
     */
    const ByteBuffer &getDictBlock() const;

    /**
     * 从区块体构建哈希树
     */
    void buildHashTree();

    /**
     * 计算blockHashOffset
     */
    static HashTreeIndex calcBlockHashOffset(DataBlockIndex size);
};


#endif //NEWS_BLOCKCHAIN_CHAINBLOCK_H
