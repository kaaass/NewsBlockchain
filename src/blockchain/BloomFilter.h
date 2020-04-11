#ifndef NEWS_BLOCKCHAIN_BLOOMFILTER_H
#define NEWS_BLOCKCHAIN_BLOOMFILTER_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include <util/ByteBuffer.h>

/**
 * 布隆过滤器的简单实现
 */
class BloomFilter {

    /**
     * 用于产生哈希函数的种子
     */
    std::vector<ULong> hashSeeds;

    /**
     * 存放比特位的缓冲区
     */
    ByteBuffer bitBuffer;

    /**
     * 比特数
     */
    size_t size = 0;

    BloomFilter() = default;

public:

    explicit BloomFilter(size_t capacity, float falsePositive = 0.02);

    /**
     * 向布隆过滤器插入内容
     * @param data
     */
    void insert(const ByteBuffer& data);

    /**
     * 判断布隆过滤器是否包含
     * @param data
     * @return
     */
    bool contain(const ByteBuffer& data);

    /**
     * 把数据写入 buffer
     * @param buffer
     */
    void writeToBuffer(ByteBuffer& buffer);

    /**
     * 从 buffer 读取
     * @param buffer
     * @return
     */
    static BloomFilter * readFromBuffer(const ByteBuffer& buffer);

private:

    size_t hash(const ByteBuffer&data, ULong seed);
};


#endif //NEWS_BLOCKCHAIN_BLOOMFILTER_H
