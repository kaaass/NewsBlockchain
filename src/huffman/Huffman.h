#ifndef NEWS_BLOCKCHAIN_HUFFMAN_H
#define NEWS_BLOCKCHAIN_HUFFMAN_H

#include <util/ByteBuffer.h>
/**
 * Huffman压缩的对外接口
 */
class Huffman {

public:

    /**
     * 压缩结果
     */
    struct Result {

        /**
         * 压缩字典
         */
        ByteBuffer dictionary;

        /**
         * 压缩结果数据块
         * 每一块对应于传入的一块
         */
        std::vector<ByteBuffer> data;
    };

    /**
     * 对若干数据块进行压缩
     *
     * 压缩字典应该根据所有传入的数据块建立，然后逐块进行压缩
     * @param data 待压缩数据
     * @return 压缩结果对象
     */
    static Result compress(const std::vector<ByteBuffer>& data);

    /**
     * 对一个数据块进行解压
     *
     * @param dict 压缩过程返回的字典
     * @param data 待压缩数据
     * @return 解压结果
     */
    static ByteBuffer decompress(const ByteBuffer& dict, const ByteBuffer& data);
};

#endif //NEWS_BLOCKCHAIN_HUFFMAN_H
