#ifndef NEWS_BLOCKCHAIN_IHASHFUNC_H
#define NEWS_BLOCKCHAIN_IHASHFUNC_H

#include <util/Int.h>
#include <util/ByteBuffer.h>

/**
 * 哈希函数接口
 */
class IHashFunc {

public:
    /**
     * 执行哈希函数
     * @param buffer 数据输入为一个字节缓冲
     * @return 返回32bit长度的哈希值
     */
    virtual UInt32 run(const ByteBuffer& buffer) = 0;
};

#endif //NEWS_BLOCKCHAIN_IHASHFUNC_H
