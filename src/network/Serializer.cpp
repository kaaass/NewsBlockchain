#include "Serializer.h"

#include <sstream>
#include <string>

using json = nlohmann::json;

void to_json(json &j, const ByteBuffer &buffer) {
    std::stringstream sstream;
    sstream << buffer;
    j = sstream.str();
}

void from_json(const json& j, ByteBuffer& buffer) {
    buffer = ByteBuffer::hex(j.get<std::string>());
}

void to_json(json &j, const ChainBlock::Header &header) {
    j = json{
            {"blockId",       (UInt) header.blockId},
            {"prevBlockHash", Serializer::uInt32Hex(header.prevBlockHash)},
            {"hashRoot",      Serializer::uInt32Hex(header.hashRoot)},
            {"timestamp",     header.timestamp},
            {"size",          header.size}
    };
}

void to_json(json &j, const ChainBlock &chainBlock) {
    // 区块头
    j["header"] = chainBlock.getHeader();
    // 哈希树
    j["hashTree"] = json::array();
    for (auto &hash: chainBlock.getHashTree()) {
        j["hashTree"].push_back(Serializer::uInt32Hex(hash));
    }
    // 区块体
    j["body"] = json::array();
    auto &blocks = chainBlock.getAllDataBlock();
    for (UInt id = 0; id < blocks.size(); id++) {
        j["body"].push_back({
                                    {"id",   id},
                                    {"size", blocks[id].size()},
                                    {"data", blocks[id]}
                            });
    }
}

std::string Serializer::uInt32Hex(UInt32 uInt32) {
    auto vec = uInt32.toVector();
    return vec[0].toHexString()
           + vec[1].toHexString()
           + vec[2].toHexString()
           + vec[3].toHexString();
}

void Serializer::decompressedBlock(nlohmann::json &j, const ChainBlock &chainBlock) {
    j = chainBlock;
    // 区块体
    j["decompressed"] = json::array();
    auto blocks = chainBlock.getAllDecomposedDataBlock();
    for (UInt id = 0; id < blocks.size(); id++) {
        j["decompressed"]
                .push_back({
                                   {"id",   id},
                                   {"size", blocks[id].size()},
                                   {"data", Serializer::bufferRawStr(blocks[id])}
                           });
    }
}

void Serializer::binaryBlock(nlohmann::json &j, const ChainBlock &chainBlock) {
    auto buffer = ByteBuffer();
    chainBlock.writeBuffer(buffer);
    j["header"] = chainBlock.getHeader();
    j["binary"] = buffer;
}

std::string Serializer::bufferRawStr(const ByteBuffer &buffer) {
    return std::string(buffer.data(), buffer.data() + buffer.size());
}
