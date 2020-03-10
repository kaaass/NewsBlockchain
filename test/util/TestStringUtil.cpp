#include <gtest/gtest.h>

#include <util/StringUtil.h>

TEST(TestStringUtil, testSplit) {
    // 函数出处相对可信
    auto ret = StringUtil::split("1, 4, abc, ", ", ");

    ASSERT_EQ(3u, ret.size());
    ASSERT_EQ("1", ret[0]);
    ASSERT_EQ("4", ret[1]);
    ASSERT_EQ("abc", ret[2]);
}

TEST(TestStringUtil, testSplitParagraph) {
    std::string para = "Para1,abc\n"
                       "233333333\n\n"
                       "Next para la la la\n";
    auto ret = StringUtil::splitParagraph(para);

    ASSERT_EQ(3u, ret.size());
    ASSERT_EQ(ByteBuffer::str("Para1,abc"), ret[0]);
    ASSERT_EQ(ByteBuffer::str("233333333"), ret[1]);
    ASSERT_EQ(ByteBuffer::str("Next para la la la"), ret[2]);
}