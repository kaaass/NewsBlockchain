#include <gtest/gtest.h>

#include <hash/CrcHash.h>

TEST(TestCrcHash, testRun) {
    ByteBuffer buffer({ 'I', 'L', 'O', 'V', 'J', 'L', 'U' });
    std::string str = "ILOVJLU";

    // 两个run的结果应相同
    ASSERT_EQ((UInt)Hash::run(buffer), (UInt)Hash::run(str));

    // 多次计算的结果应该相同
    ASSERT_EQ((UInt)Hash::run(buffer), (UInt)Hash::run(buffer));

    // 哈希值应该和整体相关
    ASSERT_NE((UInt)Hash::run("ILOVJLU!"), (UInt)Hash::run(str));

    // 简单变化后的结果应该不同
    ASSERT_NE((UInt)Hash::run("ILovJLU"), (UInt)Hash::run(str));
    ASSERT_NE((UInt)Hash::run("ILOVjlu"), (UInt)Hash::run(str));
    ASSERT_NE((UInt)Hash::run("I LOVJLU"), (UInt)Hash::run(str));
}