#include "Search.h"
#include <vector>

std::vector<ByteBuffer> Search::search(std::vector<std::string>keyWords) const {
	
	auto temp = BlockChain::getGlobalChain();//返回区块链的引用
	for (auto& blockRef : temp) 
	{
		auto realBlock = blockRef.get();
		std::vector<ByteBuffer> ret= realBlock.getAllDecomposedDataBlock();//得到所有数据块的解压数据
		//ret.
		


	}
	ChainBlock::getHeader();
	Blockchain::GLOBAL_CHAIN;
}