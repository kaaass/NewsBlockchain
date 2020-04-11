#ifndef SHA256_H
#define SHA256_H

#include "IHashFunc.h"
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Sha256 : public IHashFunc {
public:

    Sha256() = default;

    ~Sha256() = default;

    /**
     * 实现 hash 函数
     */
    UInt32 run(const ByteBuffer &buffer) override;

    // hash 算法入口函数
    vector<uint8_t> toHexCode(const ByteBuffer &buffer);

    // hash 算法
    int deal(const vector<uint8_t> &message,
             vector<uint8_t> *_digest);

protected:

    //hash加密的不同子函数设计

    //数据预处理函数
    int datapreprocess(vector<uint8_t> *message) const;

    //分解信息为512bit的数据块
    int devide64(const vector<uint8_t> &message,
                 vector<vector<uint8_t>> *heap64) const;

    //根据sha256算法设计，根据函数公式，将16*32的512bit扩展为64*32
    //扩展第17到64的32位数
    int expand48(const vector<uint8_t> &heap64,
                 vector<uint32_t> *result) const;

    //64层循环加密
    int hashtocode64(const vector<uint32_t> &result,
                     vector<uint32_t> *code64) const;

    int finalhashvalue(const vector<uint32_t> &input,
                       vector<uint8_t> *output) const;

    //SHA256算法需要的6种逻辑运算函数
    inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) const;

    inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z) const;

    inline uint32_t big_sigma0(uint32_t x) const;

    inline uint32_t big_sigma1(uint32_t x) const;

    inline uint32_t small_sigma0(uint32_t x) const;

    inline uint32_t small_sigma1(uint32_t x) const;

private:

    //必要的sha256算法常量
    static vector<uint32_t> ori_mes_digest;
    static vector<uint32_t> constvalue;

};

//6种逻辑运算函数定义

inline uint32_t Sha256::ch(uint32_t x, uint32_t y, uint32_t z) const {
    return (x & y) ^ ((~x) & z);
}

inline uint32_t Sha256::maj(uint32_t x, uint32_t y, uint32_t z) const {
    return (x & y) ^ (x & z) ^ (y & z);
}

inline uint32_t Sha256::big_sigma0(uint32_t x) const {
    return (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10);
}

inline uint32_t Sha256::big_sigma1(uint32_t x) const {
    return (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7);
}

inline uint32_t Sha256::small_sigma0(uint32_t x) const {
    return (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3);
}

inline uint32_t Sha256::small_sigma1(uint32_t x) const {
    return (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10);
}


#endif // SHA256_H