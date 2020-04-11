#ifndef NEWS_BLOCKCHAIN_HASH_H
#define NEWS_BLOCKCHAIN_HASH_H

#include "IHashFunc.h"
#include <string>

class SimpleHash;
class CrcHash;
class Sha256;
/**
 * 哈希函数管理类
 * 若要计算哈希，可以使用Hash::run函数
 */
class Hash {

    static IHashFunc *DEFAULT_HASH;

    /**
     * 获得默认哈希函数
     * @return
     */
    static IHashFunc *getDefaultHash();

public:

    /**
     * 计算字节缓冲区哈希值
     * @param buffer
     * @return
     */
    static UInt32 run(const ByteBuffer& buffer);

    /**
     * 计算字符串哈希值
     * @param buffer
     * @return
     */
    static UInt32 run(const std::string& buffer);
};

/**
 * 简单哈希函数
 * 仅占位用，请使用其他哈希函数替换
 */
class SimpleHash : public IHashFunc {

    UInt32 run(const ByteBuffer& buffer) override;
};

#endif //NEWS_BLOCKCHAIN_HASH_H
