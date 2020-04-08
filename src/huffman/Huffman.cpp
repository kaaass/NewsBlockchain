#include "Huffman.h"

Huffman::Result Huffman::compress(const std::vector<ByteBuffer> &data) {
    ByteBuffer dictionary;//频数字典ByteBuffer
    std::vector<ByteBuffer> compress_text;//压缩后的vector ByteBuffer
    std::array<BitBuffer, size_t(256)> codemap;//压缩字典（即时生成，非持久化保存的字典）
    BitBuffer longbuffer;//哨兵buffer

    std::array<UInt32, size_t(256)> count = doFrequency(data);//统计频数 OK
    HuffmanTree huffman_tree(count);//根据频数建立Huffman树  OK
    toCodeMap(huffman_tree, codemap, longbuffer);//根据Huffman树建立压缩字典 OK

    compress_private(codemap, data, compress_text, longbuffer);//进行压缩
    dictionary = toDictionaryByte(count);//频数转换为ByteBuffer作为频数字典 OK
    return Huffman::Result({dictionary, compress_text});
}

ByteBuffer Huffman::decompress(const ByteBuffer &dict, const ByteBuffer &data) {
    ByteBuffer decompress_text;
    std::array<UInt32, size_t(256)> count = fromDictionaryByte(dict);//把字典转换为频数 OK
    HuffmanTree huffman_tree(count);//根据频数建立Huffman树 OK
    decompress_private(huffman_tree, data, decompress_text);//根据huffman树进行解压
    return decompress_text;
}

Huffman::HuffmanTree::Node::Node(UInt32 len, Node *le, Node *ri, Byte val) : weight(len), left(le), right(ri),
                                                                             value(val) {
    ;
}

bool Huffman::HuffmanTree::Node::cmp::operator()(Node *&a, Node *&b) const {
    return a->weight > b->weight;
}

Huffman::HuffmanTree::Node::~Node() {
    delete this->left;
    delete this->right;
}

bool Huffman::HuffmanTree::Node::isLeaf() {
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
    std::priority_queue<Node *, std::vector<Node *>, Node::cmp> huffman_queue;
    for (size_t i = 0; i < 256; i++) {
        // 3.25改：频次为0的元素也放入优先队列中
        // 目的是作为哨兵项，处理尾部不满8bit的情况
        /*
        if (count.at(i) == 0) {
            continue;//出现频次为0的元素不放入优先队列中
        }
        */
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
            temp_count = size_t((unsigned char) (*iter)[i]);//将Byte与0~255的下标进行对应
            count.at(temp_count) = count.at(temp_count) + 1;
        }
    }
    return count;
}

ByteBuffer Huffman::toDictionaryByte(const std::array<UInt32, size_t(256)> &count) {
    //把count转换为ByteBuffer
    //具体方法：count的每一项转换为32位（4 byte）存放在ByteBuffer中
    //调用ByteBuffer类中的write函数即可
    ByteBuffer result;
    for (size_t i = 0; i < 256; i++) {
        result.write(count.at(i));
    }
    return result;
}


std::array<UInt32, size_t(256)> Huffman::fromDictionaryByte(const ByteBuffer &dictionary) {
    std::array<UInt32, size_t(256)> count;
    //ByteBuffer tempBuffer;
    //把ByteBuffer转换为count
    for (size_t i = 0; i < 256; i++) {
        //tempBuffer = dictionary.slice(ByteBuffer::Index(4 * i), ByteBuffer::Index(4 * i + 4));//从4 * i 到 4 * i + 4切割 4 个byte
        count.at(i) = fromByteBuffer(dictionary, 4 * i);//把这四个Byte转换为UInt32并赋值到count中
    }
    return count;
}

UInt32 Huffman::fromByteBuffer(const ByteBuffer &UInt32_buffer, size_t start) {
    unsigned int ret = 0;
    for (int i = 0; i < 4; i++) {
        ret |= static_cast<unsigned int>(UInt32_buffer[3 - i + start] << 8 * i);
    }
    return ret;
    //return UInt32();
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

void Huffman::toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)> &codemap, BitBuffer &longbuffer) {
    BitBuffer nowmap;
    //BitBuffer tempbit;
    //for (int i = 0; i<8; i++)tempbit.bits.push_back(0);// tempbit赋值为0x00000000
    //codemap.fill(tempbit);
    creatCodeMap(huffman_tree.root, codemap, nowmap);
    for (size_t i = 0; i < 256; i++) {
        if (codemap.at(i).bits.size() >= 8) {
            longbuffer = codemap.at(i);//寻找大于等于8bit的huffman编码，作为哨兵项
            break;
        }
    }
}

void Huffman::toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)> &codemap) {
    BitBuffer nowmap;
    creatCodeMap(huffman_tree.root, codemap, nowmap);
}

void Huffman::creatCodeMap(HuffmanTree::Node *node, std::array<BitBuffer, size_t(256)> &arr, BitBuffer &nowmap) {
    //递归函数，通过遍历huffman树的所有节点形成压缩字典
    size_t index;
    if (node == nullptr) {
        return;
    }
    if (node->isLeaf()) {
        index = size_t((unsigned char) (node->value));
        arr.at(index) = nowmap;//找到叶节点，则将该叶节点对应的Byte的编码表写入
        return;
    }
    nowmap.addBit(0);
    creatCodeMap(node->left, arr, nowmap);
    nowmap.delBit();//回溯

    nowmap.addBit(1);
    creatCodeMap(node->right, arr, nowmap);
    nowmap.delBit();//回溯
}

void Huffman::compress_private(const std::array<BitBuffer, size_t(256)> &codemap,
                               const std::vector<ByteBuffer> &ori_text,
                               std::vector<ByteBuffer> &compress_text,
                               BitBuffer &longbuffer) {
    //遍历每一个ByteBuffer的每一个Byte
    for (std::vector<ByteBuffer>::const_iterator iter = ori_text.begin(); iter != ori_text.end(); iter++) {
        ByteBuffer now_bytebuffer;
        BitBuffer now_bitbuffer;
        for (size_t i = 0; i < iter->size(); i++) {
            size_t temp_count;
            BitBuffer temp_bit;
            temp_count = size_t((unsigned char) (*iter)[i]);//将Byte与0~255的下标进行对应
            temp_bit = codemap.at(temp_count);//获取该byte对应的huffman编码
            now_bitbuffer.pushBitBuffer(temp_bit);//将该huffman编码push进bitbuffer中
        }
        now_bytebuffer = now_bitbuffer.toByteBuffer(longbuffer);
        now_bitbuffer.clear();//清空
        compress_text.push_back(now_bytebuffer);
    }
}

void Huffman::compress_private_test(const std::array<BitBuffer, size_t(256)> &codemap,
                                    const std::vector<ByteBuffer> &ori_text, std::vector<ByteBuffer> &compress_text,
                                    BitBuffer &longbuffer) {
    //遍历每一个ByteBuffer的每一个Byte
    ByteBuffer now_bytebuffer;
    BitBuffer now_bitbuffer;
    size_t temp_count;
    BitBuffer temp_bit;
    for (std::vector<ByteBuffer>::const_iterator iter = ori_text.begin(); iter != ori_text.end(); iter++) {
        for (size_t i = 0; i < iter->size(); i++) {
            temp_count = size_t((unsigned char) (*iter)[i]);//将Byte与0~255的下标进行对应
            temp_bit = codemap.at(temp_count);//获取该byte对应的huffman编码
            now_bitbuffer.pushBitBuffer(temp_bit);//将该huffman编码push进bitbuffer中
        }
        now_bytebuffer = now_bitbuffer.toByteBuffer(longbuffer);
        std::cout << now_bytebuffer << std::endl;
        system("pause");
        now_bitbuffer.clear();//清空
        //now_bitbuffer.clear();//清空
        compress_text.push_back(now_bytebuffer);
    }
}

void Huffman::decompress_private(HuffmanTree huffman_tree, const ByteBuffer &ori_text, ByteBuffer &decompress_text) {
    BitBuffer bitbuffer = BitBuffer(ori_text);//把ByteBuffer转换为BitBuffer，便于遍历
    HuffmanTree::Node *p = huffman_tree.root;//用于搜索huffman树的指针
    Bit left_bit(0);
    Bit right_bit(1);
    for (std::vector<Bit>::iterator iter = bitbuffer.bits.begin(); iter != bitbuffer.bits.end(); iter++) {
        if (*iter == left_bit) {
            p = p->left;
        } else {
            p = p->right;
        }
        if (p->isLeaf()) {
            decompress_text.push_back(p->value);
            p = huffman_tree.root;
        }
    }
}

ByteBuffer Huffman::BitBuffer::toByteBuffer(Huffman::BitBuffer &longbuffer) {
    ByteBuffer buffer;
    std::array<unsigned char, 8> byte;
    int index = 0;
    unsigned char byte_num = 0;
    for (std::vector<Bit>::const_iterator iter = this->bits.begin(); iter != this->bits.end(); iter++) {
        if (index == 8) {
            byte_num = 128 * byte.at(7) + 64 * byte.at(6) + 32 * byte.at(5) + 16 * byte.at(4) + 8 * byte.at(3) +
                       4 * byte.at(2) + 2 * byte.at(1) + 1 * byte.at(0);
            buffer.push_back(byte_num);//每8个插入一次
            index = 0;
        }
        //原：byte[index++] = *iter;
        if (iter->b == 0) {
            byte.at(7 - index) = 0;
        } else {
            byte.at(7 - index) = 1;
        }
        index++;
    }
    if (index != 8) {
        //原：for (int i = index; i < 8; i++) {
        for (int i = 0; i < 8 - index; i++) {
            byte.at(7 - index - i) = longbuffer.bits.at(i);//不满8个，剩余部分置为longbuffer的一部分
        }
        byte_num = 128 * byte.at(7) + 64 * byte.at(6) + 32 * byte.at(5) + 16 * byte.at(4) + 8 * byte.at(3) +
                   4 * byte.at(2) + 2 * byte.at(1) + 1 * byte.at(0);
        buffer.push_back(byte_num);//每8个插入一次
    }
    return buffer;
}

void Huffman::BitBuffer::addBit(Bit addbit) {
    this->bits.push_back(addbit);
}

void Huffman::BitBuffer::delBit() {
    this->bits.pop_back();
}

void Huffman::BitBuffer::pushBitBuffer(BitBuffer buffer) {
    for (std::vector<Bit>::const_iterator iter = buffer.bits.begin(); iter != buffer.bits.end(); iter++) {
        this->addBit(*iter);
    }
}

void Huffman::BitBuffer::clear() {
    std::vector<Bit>().swap(this->bits);
}

Huffman::BitBuffer::BitBuffer() {
}

Huffman::BitBuffer::BitBuffer(ByteBuffer buffer) {
    for (size_t i = 0; i < buffer.size(); i++) {
        for (size_t j = 0; j < 8; j++) {
            //原：addBit(buffer[i][j]);
            addBit(buffer[i][7 - j]);
        }
    }
}
