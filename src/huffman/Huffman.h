#ifndef NEWS_BLOCKCHAIN_HUFFMAN_H
#define NEWS_BLOCKCHAIN_HUFFMAN_H

#include <util/ByteBuffer.h>
#include <array>
#include <queue>

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
    * Huffman压缩的对外接口
    */

    /**
    * 对若干数据块进行压缩
    *
    * 压缩字典应该根据所有传入的数据块建立，然后逐块进行压缩
    * @param data 待压缩数据
    * @return 压缩结果对象
    */
    static Result compress(const std::vector<ByteBuffer> &data);
	static std::vector<ByteBuffer> compress(const ByteBuffer &dict, const std::vector<ByteBuffer> &data);
    /**
    * 对一个数据块进行解压
    *
    * @param dict 压缩过程返回的字典
    * @param data 待压缩数据
    * @return 解压结果
    */
    static ByteBuffer decompress(const ByteBuffer &dict, const ByteBuffer &data);

public:
    class HuffmanTree {
    public:
        class Node {
        public:
            Byte value;//叶子节点的Byte有效，非叶子节点的Byte无意义
            UInt32 weight;//权
            Node *left;
            Node *right;

            //std::sort的排序优先级函数
            struct cmp {
                bool operator()(Node *&a, Node *&b) const;
            };

            /*
            * 构造函数指定权，左子树，右子树，Byte
            */
            Node(UInt32 len = 0, Node *le = nullptr, Node *ri = nullptr, Byte val = 0b11111111u);

            ~Node();

            //判断当前节点是否为叶子节点
            bool isLeaf();
        };

        Node *root;//Huffman树的根节点

        HuffmanTree(std::array<UInt32, size_t(256)> count);

        void creatHuffman(std::array<UInt32, size_t(256)> count);
    };

    class BitBuffer {
    public:
        std::vector<Bit> bits;

        ByteBuffer toByteBuffer(Huffman::BitBuffer &longbuffer);

        void addBit(Bit);

        void delBit();

        void pushBitBuffer(BitBuffer buffer);

        void clear();

        BitBuffer();//默认构造函数
        BitBuffer(ByteBuffer);//转换构造函数
    };


private:
    //根据原始数据统计频数（字典）
    static std::array<UInt32, size_t(256)> doFrequency(const std::vector<ByteBuffer> &data);

    //把统计数据（字典）转换成二进制ByteBuffer
    static ByteBuffer toDictionaryByte(const std::array<UInt32, size_t(256)> &count);

    //把二进制ByteBuffer转换为统计数据（字典）
    static std::array<UInt32, size_t(256)> fromDictionaryByte(const ByteBuffer &dictionary);

    //把包含4个Byte的ByteBuffer转换为UInt32
    static UInt32 fromByteBuffer(const ByteBuffer &UInt32_buffer, size_t start);

    //把huffman树转换为编码字典
    static void toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)> &, BitBuffer &longbuffer);

    static void toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)> &);

    //递归函数遍历huffman树，创建编码字典
    static void creatCodeMap(HuffmanTree::Node *node, std::array<BitBuffer, size_t(256)> &arr, BitBuffer &nowmap);

    //根据压缩字典、原文，创建压缩后的文本
    static void compress_private(const std::array<BitBuffer, size_t(256)> &codemap,
                                 const std::vector<ByteBuffer> &ori_text,
                                 std::vector<ByteBuffer> &compress_text,
                                 BitBuffer &longbuffer);

    //根据huffman树、原文，创建解压后的文本
    static void decompress_private(HuffmanTree huffman_tree, const ByteBuffer &ori_text, ByteBuffer &decompress_text);
};

#endif //NEWS_BLOCKCHAIN_HUFFMAN_H
