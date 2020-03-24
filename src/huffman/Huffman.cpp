#include "Huffman.h"

Huffman::Result Huffman::compress(const std::vector<ByteBuffer> &data) {
	ByteBuffer dictionary;//频数字典ByteBuffer
	std::vector<ByteBuffer> compress_text;//压缩后的ByteBuffer
	std::array<BitBuffer, size_t(256)> codemap;//压缩字典（即时生成，非持久化保存的字典）

	std::array<UInt32, size_t(256)> count = doFrequency(data);//统计频数
	HuffmanTree huffman_tree(count);//根据频数建立Huffman树
	toCodeMap(huffman_tree,codemap);//根据Huffman树建立压缩字典
	//待续


	dictionary = toDictionaryByte(count);//频数转换为ByteBuffer作为频数字典
    return Huffman::Result({dictionary, compress_text});
}

ByteBuffer Huffman::decompress(const ByteBuffer &dict, const ByteBuffer &data) {
    // 待续
    return data;
}

Huffman::HuffmanTree::Node::Node(UInt32 len, Node *le, Node *ri, Byte val) :weight(len), left(le), right(ri), value(val) {
	;
}
bool Huffman::HuffmanTree::Node::cmp::operator()(Node *&a, Node *&b) const {
	return a->weight > b->weight;
}
Huffman::HuffmanTree::Node::~Node() {
	delete this->left;
	delete this->right;
}
bool Huffman::HuffmanTree::Node::isLeaf()
{
	if (this == nullptr) {
		return false;
	}
	if (this->left == nullptr && this->right == nullptr) {
		return true;
	}
	return false;
}
Huffman::HuffmanTree::HuffmanTree(std::array<UInt32, size_t(256)> count) {
	this->creatHuffman(count);
}

void Huffman::HuffmanTree::creatHuffman(std::array<UInt32, size_t(256)> count) {
	/*
	** 根据统计数据，构建一棵Huffman树
	*/

	//把256个元素放入优先队列中
	std::priority_queue<Node*, std::vector<Node*>, Node::cmp> huffman_queue;
	for (size_t i = 0; i < 256; i++) {
		if (count.at(i) == 0) {
			continue;//出现频次为0的元素不放入优先队列中
		}
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

std::array<UInt32, size_t(256)> Huffman::doFrequency(const std::vector<ByteBuffer> &data) {
	//根据原始数据统计频数
	std::array<UInt32, size_t(256)> count;//Huffman频数
	count.fill(0);//初始化清空

	//遍历每一个ByteBuffer的每一个Byte，统计频数
	for (std::vector<ByteBuffer>::const_iterator iter = data.begin(); iter != data.end(); iter++) {
		for (size_t i = 0; i < iter->size(); i++) {
			size_t temp_count;
			temp_count = size_t((unsigned char((*iter)[i])));//将Byte与0~255的下标进行对应
			count.at(temp_count) = count.at(temp_count) + 1;
		}
	}
	return count;
}

ByteBuffer Huffman::toDictionaryByte(const std::array<UInt32, size_t(256)>& count)
{
	//把count转换为ByteBuffer
	//具体方法：count的每一项转换为32位（4 byte）存放在ByteBuffer中
	//调用ByteBuffer类中的write函数即可
	ByteBuffer result;
	for (size_t i = 0; i < 256; i++) {
		result.write(count.at(i));
	}
	return result;
}


std::array<UInt32, size_t(256)> Huffman::fromDictionaryByte(ByteBuffer& dictionary)
{
	std::array<UInt32, size_t(256)> count;
	//把ByteBuffer转换为count
	for (size_t i = 0; i < 256; i++) {
		ByteBuffer tempBuffer;
		tempBuffer = dictionary.slice(4 * i, 4 * i + 4);//从4 * i 到 4 * i + 4切割 4 个byte
		count.at(i) = fromByteBuffer(tempBuffer);//把这四个Byte转换为UInt32并赋值到count中
	}
	return count;
}

UInt32 Huffman::fromByteBuffer(ByteBuffer & UInt32_buffer)
{
	unsigned int ret = 0;//此处复制UInt32私有函数的代码
	for (int i = 0; i < 4; i++) {
		ret |= static_cast<unsigned int>(UInt32_buffer[3 - i] << 8 * i);
	}
	return ret;
}

//std::array<BitBuffer, size_t(256)> Huffman::toCodeMap(HuffmanTree huffman_tree)
//{
//	return std::array<BitBuffer, size_t(256)>();
//}

/*
std::array<BitBuffer, size_t(256)> Huffman::toCodeMap(HuffmanTree huffman_tree)
{
	std::array<BitBuffer, size_t(256)> codemap;
	BitBuffer nowmap;
	BitBuffer tempbit;
	for(int i=0;i<8;i++)tempbit.bits.push_back(0);// tempbit赋值为0x00000000
	codemap.fill(tempbit);// 0x00000000作为哨兵项，防止出现编码后的数据不满整Byte
	creatCodeMap(huffman_tree.root, codemap, nowmap);
	return codemap;
}*/

void Huffman::toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)>& codemap)
{
	BitBuffer nowmap;
	BitBuffer tempbit;
	for (int i = 0; i<8; i++)tempbit.bits.push_back(0);// tempbit赋值为0x00000000
	codemap.fill(tempbit);// 0x00000000作为哨兵项，防止出现编码后的数据不满整Byte
	creatCodeMap(huffman_tree.root, codemap, nowmap);
}

void Huffman::creatCodeMap(HuffmanTree::Node * node, std::array<BitBuffer, size_t(256)>& arr, BitBuffer& nowmap)
{
	//递归函数，通过遍历huffman树的所有节点形成压缩字典
	size_t index;
	if (node == nullptr) {
		return;
	}
	if (node->isLeaf()) {
		index = size_t(unsigned char(node->value));
		arr.at(index) = nowmap;
		return;
	}
	nowmap.bits.push_back(0);
	creatCodeMap(node->left, arr, nowmap);
	nowmap.bits.pop_back();

	nowmap.bits.push_back(1);
	creatCodeMap(node->right, arr, nowmap);
	nowmap.bits.pop_back();
}

void Huffman::compress_private(const std::array<BitBuffer, size_t(256)>& codemap, const std::vector<ByteBuffer>& ori_text, std::vector<ByteBuffer>& compress_text)
{
	//遍历每一个ByteBuffer的每一个Byte
	for (std::vector<ByteBuffer>::const_iterator iter = ori_text.begin(); iter != ori_text.end(); iter++) {
		ByteBuffer now_bytebuffer;
		BitBuffer now_bitbuffer;
		for (size_t i = 0; i < iter->size(); i++) {
			size_t temp_count;
			BitBuffer temp_bit;
			temp_count = size_t((unsigned char((*iter)[i])));//将Byte与0~255的下标进行对应
			temp_bit = codemap.at(temp_count);//获取该byte对应的huffman编码
			//待续
		}
		compress_text.push_back(now_bytebuffer);
	}
}

ByteBuffer Huffman::BitBuffer::toByteBuffer()
{
	//待续
	return ByteBuffer();
}

void Huffman::BitBuffer::addBit(Bit addbit)
{
	this->bits.push_back(addbit);
}
