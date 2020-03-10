#include "Huffman.h"
#include <array>
Huffman::Result Huffman::compress(const std::vector<ByteBuffer> &data) {
    // pro1 遍历容器，统计次数

	//count存放字节的出现频数
	std::array<UInt32, size_t(256)> count;
	count.fill(0);

	//遍历每一个ByteBuffer
	for (std::vector<ByteBuffer>::const_iterator iter = data.begin; iter != data.end; iter++) {
		//遍历某一ByreBuffer的每一个Byte
		for (size_t i = 0; i < iter->size(); i++) {
			count.at(size_t((unsigned char())));
		}



	}
    return Huffman::Result({ByteBuffer({0x00}), data});
}

ByteBuffer Huffman::decompress(const ByteBuffer &dict, const ByteBuffer &data) {
    // 自行发挥
    return data;
}
