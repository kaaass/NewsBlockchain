#ifndef NEWS_BLOCKCHAIN_MURMURHASH_H
#define NEWS_BLOCKCHAIN_MURMURHASH_H

#include <cstdint>

#if defined(_MSC_VER)

#define BIG_CONSTANT(x) (x)

#else

#define BIG_CONSTANT(x) (x##LLU)

#endif

/**
 * 计算连续数据的 MurmurHash
 *
 * 算法采用 MurmurHash2，是一种分布较为随机的哈希算法。采用的实现为
 *  MurmurHash64A，代码参考自：
 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
 * 原作者为：Austin Appleby
 */
class MurmurHash {

public:
    static uint64_t run(const void *key, int len, uint64_t seed) {
        const uint64_t m = BIG_CONSTANT(0xc6a4a7935bd1e995);
        const int r = 47;

        uint64_t h = seed ^(len * m);

        const uint64_t *data = (const uint64_t *) key;
        const uint64_t *end = data + (len / 8);

        while (data != end) {
            uint64_t k = *data++;

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;
        }

        const unsigned char *data2 = (const unsigned char *) data;

        switch (len & 7) {
            case 7:
                h ^= uint64_t(data2[6]) << 48;
            case 6:
                h ^= uint64_t(data2[5]) << 40;
            case 5:
                h ^= uint64_t(data2[4]) << 32;
            case 4:
                h ^= uint64_t(data2[3]) << 24;
            case 3:
                h ^= uint64_t(data2[2]) << 16;
            case 2:
                h ^= uint64_t(data2[1]) << 8;
            case 1:
                h ^= uint64_t(data2[0]);
                h *= m;
        };

        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }
};


#endif //NEWS_BLOCKCHAIN_MURMURHASH_H
