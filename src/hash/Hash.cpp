#include "Hash.h"
#include "CrcHash.h"
#include <cstdlib>
#include "Sha256.h"

IHashFunc *Hash::DEFAULT_HASH = nullptr;

IHashFunc *Hash::getDefaultHash() {
    return new Sha256;
}

UInt32 Hash::run(const ByteBuffer &buffer) {
    if (DEFAULT_HASH == nullptr) {
        DEFAULT_HASH = getDefaultHash();
    }
    return DEFAULT_HASH->run(buffer);
}

UInt32 Hash::run(const std::string &buffer) {
    return run(ByteBuffer((void *) buffer.c_str(), buffer.size()));
}

UInt32 SimpleHash::run(const ByteBuffer& buffer) {
    if (buffer.size() <= 0)
        return 0;
    UInt nonce = buffer[0];
    for (size_t i = 1; i < buffer.size(); i++) {
        nonce = (nonce << 1u) ^ buffer[i];
    }
    srand(nonce);
    // 取第buffer.size()次的随机结果，引入长度因素
    for (size_t i = 1; i < buffer.size(); i++)
        rand();
    return rand();
}
