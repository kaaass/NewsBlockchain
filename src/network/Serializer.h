#ifndef NEWS_BLOCKCHAIN_SERIALIZER_H
#define NEWS_BLOCKCHAIN_SERIALIZER_H

#include <json.hpp>
#include <blockchain/ChainBlock.h>

/*
 * Json序列化函数
 */

void to_json(nlohmann::json &j, const ByteBuffer &buffer);

void to_json(nlohmann::json &j, const ChainBlock::Header &header);

void to_json(nlohmann::json &j, const ChainBlock &chainBlock);

/**
 * 序列化工具类
 */
class Serializer {
public:
    static std::string uInt32Hex(UInt32 uInt32);

    static std::string bufferRawStr(const ByteBuffer& buffer);

    static void decompressedBlock(nlohmann::json &node, const ChainBlock &chainBlock);

    static void binaryBlock(nlohmann::json &node, const ChainBlock &chainBlock);
};


#endif //NEWS_BLOCKCHAIN_SERIALIZER_H
