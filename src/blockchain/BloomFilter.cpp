#include <cmath>
#include <util/MurmurHash.h>
#include "BloomFilter.h"

/**
 * 创建布隆过滤器
 * @param capacity
 * @param falsePositive
 */
BloomFilter::BloomFilter(size_t capacity, float falsePositive) {

    // 计算布隆过滤器的长度
    // 公式推导见：https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions
    size = std::ceil(-(double) capacity * std::log(falsePositive) / (std::log(2) * std::log(2)));
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

void BloomFilter::insert(const std::string &str) {
    size_t pos;
    for (auto seed : hashSeeds) {
        pos = hash(str.data(), str.length(), seed);
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

bool BloomFilter::contain(const std::string &str) {
    size_t pos;
    for (auto seed : hashSeeds) {
        pos = hash(str.data(), str.length(), seed);
        if (!(bitBuffer[pos >> 3u] & (1u << (pos & 3u))))
            return false;
    }
    return true;
}

size_t BloomFilter::hash(const ByteBuffer &data, ULong seed) {
    return MurmurHash::run(data.data(), data.size(), seed) % size;
}

size_t BloomFilter::hash(const void *data, size_t len, ULong seed) {
    return MurmurHash::run(data, len, seed) % size;
}

void BloomFilter::writeToBuffer(ByteBuffer &buffer) {
    ULong temp = 0;
    buffer.push_back(&size, 8u);
    // Seeds
    temp = hashSeeds.size();
    buffer.push_back(&temp, 8u);
    for (auto seed : hashSeeds) {
        buffer.push_back(&seed, 8u);
    }
    // Bits
    temp = bitBuffer.size();
    buffer.push_back(&temp, 8u);
    buffer.push_back(bitBuffer.data(), bitBuffer.size());
}

BloomFilter *BloomFilter::readFromBuffer(const ByteBuffer &buffer) {
    auto *bf = new BloomFilter();
    size_t offset = 0;
    size_t size = 0;
    // Size
    bf->size = *reinterpret_cast<const ULong *>(buffer.data());
    offset += 8u;
    // Seeds
    size = *reinterpret_cast<const ULong *>(buffer.data() + offset);
    offset += 8u;
    for (size_t i = 0; i < size; i++) {
        bf->hashSeeds.push_back(*reinterpret_cast<const ULong *>(buffer.data() + offset));
        offset += 8u;
    }
    // Bits
    size = *reinterpret_cast<const ULong *>(buffer.data() + offset);
    offset += 8u;
    bf->bitBuffer.allocate(size);
    for (size_t i = 0; i < size; i++) {
        bf->bitBuffer[i] = *(buffer.data() + offset);
        offset++;
    }
    return bf;
}
