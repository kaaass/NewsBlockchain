#ifndef NEWS_BLOCKCHAIN_BYTEBUFFER_H
#define NEWS_BLOCKCHAIN_BYTEBUFFER_H

#include <util/Types.h>
#include <vector>
#include <string>

/**
 * 字节缓冲区
 */
class ByteBuffer {

    std::vector<Byte> bytes;

public:

    typedef long Index;

    ByteBuffer() = default;

    ByteBuffer(void *data, size_t size);

    /**
     * 将字节vector转为字节缓冲区
     * @param buffer
     */
    ByteBuffer(const std::vector<Byte> &buffer);

    /**
     * 将字符串转为字节缓冲区
     * @param buffer
     */
    static ByteBuffer str(const std::string &str);

    ByteBuffer &operator=(const std::vector<Byte> &buffer);

    /**
     * 返回当前缓冲区大小
     * @return
     */
    size_t size() const;

    /**
     * 在缓冲区末尾插入一个字节
     * @param byte
     * @return
     */
    ByteBuffer &push_back(Byte byte);

    /**
     * 在缓冲区末尾插入一段连续内存
     * @param data
     * @param size
     * @return
     */
    ByteBuffer &push_back(void *data, size_t size);

    /**
     * 在缓冲区头插入一个字节
     * @param byte
     * @return
     */
    ByteBuffer &push_front(Byte byte);

    /**
     * 在缓冲区头插入一段连续内存
     * @param data
     * @param size
     * @return
     */
    ByteBuffer &push_front(void *data, size_t size);

    /**
     * 拼接缓冲区
     * @param buffer 下标，为负即从尾部取
     * @return 返回新的缓冲区
     */
    ByteBuffer operator+(const ByteBuffer &buffer) const;

    /**
     * 取缓冲区一个字节
     * @param ind
     * @return
     */
    Byte &operator[](Index ind);

    Byte operator[](Index ind) const;

    /**
     * 将缓冲区数据转为vector
     * @return 复制数据为新vector
     */
    std::vector<Byte> toVector() const;

    /**
     * 返回缓冲区数据首指针
     *
     * 使用时应该结合ByteBuffer::size返回的尺寸
     * @return
     */
    Byte *data();

    /**
     * 创建缓冲区的分片
     *
     * @param start 开始下标，从开头取至结尾，为负即从尾部取
     * @return 返回新的缓冲区，大小为size-start
     */
    ByteBuffer slice(Index start);

    /**
     * 创建缓冲区的分片
     *
     * @param start 开始下标，为负即从尾部取
     * @param end 结束下标，即结束元素下标+1，为负即从尾部取
     * @return 返回新的缓冲区，大小为end-start
     */
    ByteBuffer slice(Index start, Index end);

    /**
     * 在缓冲区末尾以大端序插入一个16位无符号整数
     * @param byte
     * @return
     */
    ByteBuffer &write(UShort byte);

    /**
     * 在缓冲区末尾以大端序插入一个32位无符号整数
     * @param byte
     * @return
     */
    ByteBuffer &write(UInt byte);

    /**
     * 在缓冲区末尾以大端序插入一个64位无符号整数
     * @param byte
     * @return
     */
    ByteBuffer &write(ULong byte);

    friend bool operator==(const ByteBuffer &, const ByteBuffer &);

    friend std::ostream &operator<<(std::ostream &, const ByteBuffer &);

    ~ByteBuffer() = default;
};

/**
 * 写字节缓冲区接口
 */
class IByteBufferWriter {

public:

    /**
     * 向字节缓冲区中写入对象数据
     * @param buffer
     * @return
     */
    virtual ByteBuffer &writeBuffer(ByteBuffer &buffer) = 0;
};


#endif //NEWS_BLOCKCHAIN_BYTEBUFFER_H
