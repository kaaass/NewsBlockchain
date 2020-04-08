#pragma once
#ifndef NEWS_BLOCKCHAIN_SEARCH_H
#define NEWS_BLOCKCHAIN_SEARCH_H


#include <blockchain/ChainBlock.h>
#include <blockchain/Blockchain.h>
#include <algorithm>


/*
#ifdef UNIT_TEST

#include <gtest/gtest.h>

#endif
*/


#define MAXNUM 256

/*
* 关键字搜索接口
*/

class Search {


    static std::vector<int> moveLenth;
public:

    struct Max {
        int count;
        UInt32 id;
    };

    /*
    * 关键字搜索函数，返回关键字出现次数最多的新闻
    * 输入关键字
    * 返回整个区块体（除字典）
    */

    static std::vector<UInt32> search(std::vector<std::string> &keyWords);


    /*
    *  Sunday算法
    *  对某一区块进行关键字查找
    *  返回对某一关键字在该区块中匹配的次数
    */

    static int Sunday(ByteBuffer &key, std::vector<ByteBuffer> &block);


    /*
    *得到sunday算法中模式串的偏移量
    */

    static void getMovelenth(ByteBuffer &key);

};

int location(int a, int b, int c, int d);

bool isChar(Byte &b);

#endif // NEWS_BLOCKCHAIN_SEARCH_H


