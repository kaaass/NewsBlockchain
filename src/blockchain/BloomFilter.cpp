#include <cmath>
#include <util/MurmurHash.h>
#include "BloomFilter.h"

/**
 * 创建布隆过滤器
 * @param capacity
 * @param falsePositive
 */
BloomFilter::BloomFilter(size_t capacity, float falsePositive) :
        capacity(capacity), falsePositive(falsePositive) {

    // 计算布隆过滤器的长度
    // 公式推导见：https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions
    size = std::ceil(- (double) capacity * std::log(falsePositive) / (std::log(2) * std::log(2)));
    bitBuffer.allocate((size >> 3u) + 1);

    // 计算哈希函数个数
    size_t n = std::ceil(std::log(2) * size / capacity);
    hashSeeds.assign(n, 0);

    // 生成哈希种子
    // 使用最常见的线性同余器
    UInt a = 1664525u;
    UInt c = 1013904223u;
    UInt x = 314159265u;
    for (size_t i = 0; i < n; i++) {
        hashSeeds[i] = x;
        x = a * x + c;
    }
}

void BloomFilter::insert(const ByteBuffer &data) {
    size_t pos;
    for (auto seed : hashSeeds) {
        pos = hash(data, seed);
        bitBuffer[pos >> 3u] = bitBuffer[pos >> 3u] | (1u << (pos & 3u));
    }
}

bool BloomFilter::contain(const ByteBuffer &data) {
    size_t pos;
    for (auto seed : hashSeeds) {
        pos = hash(data, seed);
        if (!(bitBuffer[pos >> 3u] & (1u << (pos & 3u))))
            return false;
    }
    return true;
}

size_t BloomFilter::hash(const ByteBuffer &data, ULong seed) {
    return MurmurHash::run(data.data(), data.size(), seed) % size;
}
