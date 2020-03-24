#include <gtest/gtest.h>

#include <search/Search.h>


TEST(testSearch,testSunday) {
	
	ByteBuffer key;//chushihua
	key=ByteBuffer::str("abc");
	
	std::vector<ByteBuffer> block;
	ByteBuffer a = ByteBuffer::str("abc");
	ByteBuffer b = ByteBuffer::str("  cdfabcw abc");
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

TEST(testSearch, testsearch)
{
	
	Blockchain::create("abc\nabcc\nccc\nabcc abcc\nac \nccc");
	Blockchain::create("ccc abcb\nabc\nac");
	Blockchain::create("cac\nabcabc\nac");
	ASSERT_EQ(3u, Blockchain::size());
	std::vector<std::string> key;
	key.push_back("abc");
	key.push_back("ac");
	key.push_back("ccc");
	ASSERT_EQ( (UInt32)2, Search::search(key).size());
	
}


// 自行发挥