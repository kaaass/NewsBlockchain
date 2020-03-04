#include "Huffman.h"

Huffman::Result Huffman::compress(const std::vector<ByteBuffer> &data) {
    // 自行发挥
    return Huffman::Result({ByteBuffer({0x00}), data});
}

ByteBuffer Huffman::decompress(const ByteBuffer &dict, const ByteBuffer &data) {
    // 自行发挥
    return data;
}
