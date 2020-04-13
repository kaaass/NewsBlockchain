#include <gtest/gtest.h>

#include <util/Types.h>
#include <util/ByteBuffer.h>

TEST(TestByteBuffer, testConstruct) {
    // 测试vector
    std::vector<Byte> vec = {0x23u, 0x19u, 0x51u};
    ByteBuffer bufferA(vec);
    ASSERT_EQ(3u, bufferA.size());
    for (ULong i = 0; i < vec.size(); i++)
        ASSERT_EQ(vec[i], bufferA[i]);

    // 测试连续内存
    Byte data[] = {0x15, 0x23, 0x72, 0x6f, 0x00};
    ByteBuffer bufferB(data, 5u);
    ASSERT_EQ(5u, bufferB.size());
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(data[i], bufferB[i]);

    // 测试字符串
    std::string str = "abcde";
    ByteBuffer bufferC = ByteBuffer::str(str);
    ASSERT_EQ(5u, bufferC.size());
    for (ULong i = 0; i < str.size(); i++)
        ASSERT_EQ(str[i], bufferC[i]);
}

TEST(TestByteBuffer, testOperate) {
    // 测试==
    ASSERT_TRUE(ByteBuffer({0x23}) == ByteBuffer({0x23}));
    ASSERT_TRUE(ByteBuffer({0x12, 0x34}) == ByteBuffer({0x12, 0x34}));

    // 测试+
    ByteBuffer bufferA({0x12, 0x19, 0x50, 0x3f});
    ByteBuffer bufferB({0x4f, 0xff});
    ByteBuffer bufferAB = bufferA + bufferB;
    // A、B内容不变
    ASSERT_EQ(ByteBuffer({0x12, 0x19, 0x50, 0x3f}), bufferA);
    ASSERT_EQ(ByteBuffer({0x4f, 0xff}), bufferB);
    // AB为连接的内容
    ASSERT_EQ(6u, bufferAB.size());
    ASSERT_EQ(ByteBuffer({0x12, 0x19, 0x50, 0x3f, 0x4f, 0xff}), bufferAB);
}

TEST(TestByteBuffer, testInsert) {
    ByteBuffer buffer;
    Byte data[] = {0x15, 0x23, 0x72, 0x6f, 0x00};

    // 测试push_back
    buffer = {0x12, 0x19, 0x50, 0x3f};
    buffer.push_back(0x26);
    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ(0x26, buffer[4]);

    buffer.push_back(data, 5);
    ASSERT_EQ(10u, buffer.size());
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(data[i], buffer[5 + i]);

    // 测试push_front
    buffer = {0x12, 0x19, 0x50, 0x3f};
    buffer.push_front(0x19);
    ASSERT_EQ(5u, buffer.size());
    ASSERT_EQ(0x19, buffer[0]);

    buffer.push_front(data, 5);
    ASSERT_EQ(10u, buffer.size());
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(data[i], buffer[i]);
}

TEST(TestByteBuffer, testSlice) {
    ByteBuffer buffer({0x12, 0x19, 0x50, 0x3f, 0x4f, 0xff});

    // 测试切片
    ASSERT_EQ(ByteBuffer({0x19}), buffer.slice(1, 2));
    ASSERT_EQ(ByteBuffer({0xff}), buffer.slice(-1));
    ASSERT_EQ(ByteBuffer({0x3f, 0x4f, 0xff}), buffer.slice(3));
    ASSERT_EQ(ByteBuffer({0x19, 0x50, 0x3f, 0x4f}), buffer.slice(1, -1));
    ASSERT_EQ(ByteBuffer({0x50, 0x3f}), buffer.slice(-4, -2));
}

TEST(TestByteBuffer, testOutput) {
    std::stringstream sstream;

    ByteBuffer buffer({0x12, 0x19, 0x50, 0x3f, 0x4f, 0x0, 0xff});
    sstream << buffer;
    ASSERT_EQ("12 19 50 3f 4f 00 ff", sstream.str());

    buffer = ByteBuffer::hex("12 19 50");
    ASSERT_EQ(ByteBuffer({0x12, 0x19, 0x50}), buffer);
}

TEST(TestByteBuffer, testWrite) {
    ByteBuffer buf;

    // UShort
    UShort ushort = 0x1234u;
    buf.write(ushort);
    ASSERT_EQ(2u, buf.size());
    ASSERT_EQ(0x12u, buf[0]);
    ASSERT_EQ(0x34u, buf[1]);

    // UInt
    buf = ByteBuffer();
    UInt uInt = 0x12345678u;
    buf.write(uInt);
    ASSERT_EQ(4u, buf.size());
    ASSERT_EQ(0x12u, buf[0]);
    ASSERT_EQ(0x34u, buf[1]);
    ASSERT_EQ(0x56u, buf[2]);
    ASSERT_EQ(0x78u, buf[3]);

    // ULong
    buf = ByteBuffer();
    auto uLong = (ULong) 0x1234567887654321u;
    buf.write(uLong);
    ASSERT_EQ(8u, buf.size());
    ASSERT_EQ(0x12u, buf[0]);
    ASSERT_EQ(0x34u, buf[1]);
    ASSERT_EQ(0x56u, buf[2]);
    ASSERT_EQ(0x78u, buf[3]);
    ASSERT_EQ(0x87u, buf[4]);
    ASSERT_EQ(0x65u, buf[5]);
    ASSERT_EQ(0x43u, buf[6]);
    ASSERT_EQ(0x21u, buf[7]);
}