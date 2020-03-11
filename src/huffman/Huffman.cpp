#include "Huffman.h"
#include <array>
#include <queue>
Huffman::Result Huffman::compress(const std::vector<ByteBuffer> &data) {

	HuffmanTree huffman_tree(data);//建立Huffman树


    return Huffman::Result({ByteBuffer({0x00}), data});
}

ByteBuffer Huffman::decompress(const ByteBuffer &dict, const ByteBuffer &data) {
    // 自行发挥
    return data;
}

Huffman::HuffmanTree::Node::Node(ULong len, Node *le, Node *ri, Byte val) :weight(len), left(le), right(ri), value(val) {
	;
}
bool Huffman::HuffmanTree::Node::cmp::operator()(Node *&a, Node *&b) const {
	return a->weight > b->weight;
}
Huffman::HuffmanTree::Node::~Node() {
	delete this->left;
	delete this->right;
}
Huffman::HuffmanTree::HuffmanTree(const std::vector<ByteBuffer> &data) {
	this->creatHuffman(data);
}

void Huffman::HuffmanTree::creatHuffman(const std::vector<ByteBuffer> &data) {
	/*
	** 以下构建一棵Huffman树
	*/

	//count存放字节的出现频数，并初始化
	std::array<ULong, size_t(256)> count;
	count.fill(0);

	//遍历每一个ByteBuffer的每一个Byte，统计频数
	for (std::vector<ByteBuffer>::const_iterator iter = data.begin(); iter != data.end(); iter++) {
		for (size_t i = 0; i < iter->size(); i++) {
			count.at(size_t((unsigned char((*iter)[i]))))++;
		}
	}

	//把256个元素放入优先队列中
	std::priority_queue<Node*, std::vector<Node*>, Node::cmp> huffman_queue;
	for (size_t i = 0; i < 256; i++) {
		huffman_queue.push(new Node(count.at(i), nullptr, nullptr, i));
	}

	//每次取最小的两个元素合并为新的元素，放入队列中，直至优先队列中仅有1个元素
	while (huffman_queue.size() > 1) {
		Node *p1, *p2;
		p1 = huffman_queue.top();
		huffman_queue.pop();
		p2 = huffman_queue.top();
		huffman_queue.pop();
		huffman_queue.push(new Node(p1->weight + p2->weight, p1, p2));
	}

	//仅剩的元素作为HuffmanTree的root
	this->root = huffman_queue.top();
	huffman_queue.pop();
}