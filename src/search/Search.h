#ifndef NEWS_BLOCKCHAIN_SEARCH_H
#define NEWS_BLOCKCHAIN_SEARCH_H

#include <util/Types.h>
#include <blockchain/ChainBlock.h>
#include <blockchain/Blockchain.h>
#include <vector>
#include <string>
#include <util/StringUtil.h>
#include <util/Int.h>

/*
* 关键字搜索接口
*/
class Search {
	
public:
	
	struct Max{
		int count;
		UInt32 id;
	};
	/*
	* 关键字搜索函数，返回关键字出现次数最多的新闻
	* 输入关键字
	* 返回整个区块体（除字典）
	*/
	
	static std::vector<ByteBuffer>  search(std::vector<std::string>keyWords);


	/*
	*  Sunday算法
	*  对某一区块进行关键字查找
	*  返回对某一关键字在该区块中匹配的次数
	*/

	static int Sunday(ByteBuffer &, std::vector<ByteBuffer> &);

}










#endif


