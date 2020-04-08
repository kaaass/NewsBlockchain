#include "Search.h"

std::vector<int> Search::moveLenth;

std::vector<UInt32> Search::search(std::vector<std::string> &keyWords) {

    Max max{0, -1}, second{0, -1}, third{0, -1};
    std::vector<ByteBuffer> key;//chushihua
    if (keyWords.empty()) {
        std::vector<UInt32> nullId;
        return nullId;
    }

    //字符串转字节流
    for (auto const &value : keyWords) {
        key.push_back(ByteBuffer::str(value));
    }
    int n = 0;
    auto temp = Blockchain::getGlobalChain();//返回区块链的引用
    for (auto &blockRef : temp) {
        auto realBlock = blockRef.get();
        if (realBlock.hasKeyword(keyWords)) {
            std::vector<ByteBuffer> ret = realBlock.getAllDecomposedDataBlock();//得到所有数据块的解压数据
            int total = 0, num = 0;
            for (auto &k : key) {

                num = Sunday(k, ret);
                if (num == 0) {
                    n = 0;
                    break;
                } else {
                    total += num;
                    n = 1;
                }
            }
            if (n) {
                switch (location(total, max.count, second.count, third.count)) {
                    case 1: {
                        third = second;
                        second = max;
                        max.count = total;
                        max.id = realBlock.getHeader().blockId;
                    }
                        break;
                    case 2: {
                        third = second;
                        second.count = total;
                        second.id = realBlock.getHeader().blockId;
                    }
                        break;
                    case 3: {
                        third.count = total;
                        third.id = realBlock.getHeader().blockId;
                    }
                        break;
                    default:
                        break;
                }

            }
        }
    }
    std::vector<UInt32> result;
    if (max.count)
        result.push_back(max.id);
    if (second.count)
        result.push_back(second.id);
    if (third.count)
        result.push_back(third.id);
    return result;
}

int Search::Sunday(ByteBuffer &key, std::vector<ByteBuffer> &block) {
    getMovelenth(key);
    ByteBuffer newByteBuffer;
    for (auto i = 0u; i < block.size(); i++) {
        if (i != 0u)
            newByteBuffer.push_back(' ');
        newByteBuffer = newByteBuffer.operator+(block.operator[](i));//拼接缓冲区
    }
    auto k = key.data();
    auto b = newByteBuffer.data();
    int klen = key.size();
    int bufflen = newByteBuffer.size();
    bool flag = false;
    int i = 0, count = 0, j;
    while (i < bufflen) {
        j = 0;

        for (; j < klen && i + j < bufflen && b[i + j] == k[j]; ++j);

        if (j >= klen && (i + j >= bufflen || !isChar(b[i + j]))) {
            count++;
            flag = true;
        }
        if (i + klen >= bufflen)
            return count;

        if (flag) {

            for (++j; i + j < bufflen && !isChar(b[i + j]); j++);
            i += j;
            flag == false;
        } else {
            for (j; i + j < bufflen && isChar(b[i + j]); j++);
            for (; i + j < bufflen && !isChar(b[i + j]); j++);
            if (i + j >= bufflen)
                return count;
            else
                //i += moveLenth[b[i + j]];
                i += j;

        }

    }

}

void Search::getMovelenth(ByteBuffer &key) {
    int klen = key.size();

    for (auto i = 0; i < MAXNUM; i++)
        moveLenth.push_back(klen + 1);
    for (auto i = 0; i < klen; i++)
        moveLenth[key.operator[](i)] = klen - i;
}

int location(int a, int b, int c, int d) {
    if (a > b)
        return 1;
    if (a > c)
        return 2;
    if (a > d)
        return 3;
    return 0;

}

bool isChar(Byte &b) {

    if ((b <= 'Z' && b >= 'A') || (b <= 'z' && b >= 'a') || (b <= '9' && b >= '0'))
        return true;
    else
        return false;
}
