#ifndef NEWS_BLOCKCHAIN_CRCHASH_H
#define NEWS_BLOCKCHAIN_CRCHASH_H

#include "IHashFunc.h"
//#include <string>

class CrcHash : public IHashFunc {

	/*
	*Ä¬ÈÏhashº¯Êý
	*
	*/
	UInt32 run(const ByteBuffer& buffer) override;

};



#endif //NEWS_BLOCKCHAIN_CRCHASH_H