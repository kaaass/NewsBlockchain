#include <gtest/gtest.h>

#include <search/Search.h>


TEST(testSearch,testSunday) {
	
	ByteBuffer key;//chushihua
	key=ByteBuffer::str("abc");
	
	std::vector<ByteBuffer> block;
	ByteBuffer a = ByteBuffer::str("aaaabc");
	ByteBuffer b = ByteBuffer::str("cdfabcw");
	block.push_back(a);
	block.push_back(b);
	ByteBuffer* c = &block[0];
	int num=Search::Sunday(key, block);//应该为2
	 // 两个的结果应相同
	ASSERT_EQ((UInt)2, (UInt)num); //
	ASSERT_EQ((UInt)2, (UInt)block.size());
	//ASSERT_EQ((UInt)c, (UInt)block.data());
}

TEST(testSearch, testgetMovelenth)
{
	
	std::vector<int> move;
	ByteBuffer key({ 'a', 'b', 'c' });
	int klen = key.size();

	for (auto i = 0; i < MAXNUM; i++)
		move.push_back(klen + 1);
	for (auto i = 0; i < klen; i++)
		move[key.operator[](i)] = klen - i;
	ASSERT_EQ(move['a'], 3);
	ASSERT_EQ(move['b'], 2);
	ASSERT_EQ(move[key.operator[](2)], 1);
	
}


// 自行发挥