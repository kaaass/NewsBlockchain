#include "Search.h"
#include <vector>

std::vector<ByteBuffer> Search::search(std::vector<std::string>keyWords) {

	Max max{0, 0};
	std::vector<ByteBuffer> key;//chushihua
	if (keyWords.empty())
		return;
	//字符串转字节流
	for (auto const& value : keyWords)
	{
		key.push_back(ByteBuffer::str(value));
	}

	//ByteBuffer& key = ByteBuffer::str(str);//得到关键字字节缓冲区


	auto temp = Blockchain::getGlobalChain();//返回区块链的引用
	for (auto& blockRef : temp) 
	{
		auto realBlock = blockRef.get();
		if (realBlock.hasKeyword())
		{
			std::vector<ByteBuffer> ret = realBlock.getAllDecomposedDataBlock();//得到所有数据块的解压数据
			int total = 0,num=0;
			for (auto& k : key)
			{
				num = Sunday(k, ret);
				if (num == 0)
					break;
				else
					total += num;
			}
			if(total>max.count)
			{
				max.count = total;
				max.id = realBlock.getHeader().blockId;
			}
			
			
		}


	}
	
	if (max.count)
		return Blockchain::get(max.id).getAllDecomposedDataBlock();
}

int Search::Sunday(ByteBuffer & key, std::vector<ByteBuffer> & block)
{

	return 0;
}

