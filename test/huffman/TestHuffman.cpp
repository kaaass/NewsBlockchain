#include <gtest/gtest.h>

#include <huffman/Huffman.h>

TEST(TestHuffman, testRun) {
	ByteBuffer buffer;
	std::vector<ByteBuffer> buffer_vector;
	std::string str = "Anti Tracks is a complete solution \
to protect your privacy and enhance your PC performance. \
With a simple click Anti Tracks securely erase your internet \
tracks, computer activities and programs history information \
stored in many hidden files on your computer.Anti Tracks support \
Internet Explorer, AOL, Netscape/Mozilla and Opera browsers. \
It also include more than 85 free plug-ins to extend erasing \
features to support popular programs such as ACDSee, Acrobat \
Reader, KaZaA, PowerDVD, WinZip, iMesh, Winamp and much more. \
Also you can easily schedule erasing tasks at specific time intervals \
or at Windows stat-up/ shutdown.To ensure maximum privacy protection \
Anti Tracks implements the US Department of Defense DOD 5220.22-M, \
Gutmann and NSA secure erasing methods, making any erased files \
unrecoverable even when using advanced recovery tools.";
	for (std::string::iterator iter = str.begin(); iter != str.end(); iter++) {
		buffer.push_back(*iter);
	}
	buffer_vector.push_back(buffer);
	Huffman::Result result = Huffman::compress(buffer_vector);
	ByteBuffer ans = Huffman::decompress(result.dictionary, result.data.at(0));
	std::string str2;
	for (size_t i = 0; i < buffer.size(); i++) {
		str2.push_back(ans[i]);
	}
	ASSERT_EQ((UInt)str.compare(str2), (UInt)0);//加密解密后得到的字符串相等
}

// 自行发挥