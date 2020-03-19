#ifndef NEWS_BLOCKCHAIN_SEARCH_H
#define NEWS_BLOCKCHAIN_SEARCH_H

#include <util/Types.h>
#include <blockchain/ChainBlock.h>
#include <blockchain/Blockchain.h>
#include <vector>
#include <string>


/*
* 关键字搜索接口
*/
class Search() {
	
public:
	

	/*
	* 关键字搜索函数，返回关键字出现次数最多的新闻
	* 输入关键字
	* 返回整个区块体（除字典）
	*/
	
	static std::vector<ByteBuffer> search(std::vector<string>keyWords);


}










#endif


