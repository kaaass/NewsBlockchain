#include <cassert>
#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(void *data, size_t size) {
    bytes.assign((UChar *) data, (UChar *) data + size);
}

ByteBuffer::ByteBuffer(const std::vector<Byte>& buffer) {
    bytes = buffer;
}

size_t ByteBuffer::size() const {
    return bytes.size();
}

ByteBuffer &ByteBuffer::push_back(Byte byte) {
    return push_back(&byte, 1u);
}

ByteBuffer &ByteBuffer::push_back(void *data, size_t size) {
    bytes.insert(bytes.end(), (UChar *) data, (UChar *) data + size);
    return *this;
}

ByteBuffer &ByteBuffer::push_front(Byte byte) {
    return push_front(&byte, 1u);
}

ByteBuffer &ByteBuffer::push_front(void *data, size_t size) {
    bytes.insert(bytes.begin(), (UChar *) data, (UChar *) data + size);
    return *this;
}

ByteBuffer ByteBuffer::operator+(const ByteBuffer &buffer) const {
    ByteBuffer newBuffer;
    newBuffer.bytes = bytes;
    newBuffer.bytes.insert(newBuffer.bytes.end(), buffer.bytes.begin(), buffer.bytes.end());
    return newBuffer;
}

std::ostream &operator<<(std::ostream &out, const ByteBuffer &buffer) {
    for (ULong i = 0; i < buffer.bytes.size(); i++) {
        if (i > 0)
            out << " ";
        out << buffer.bytes[i].toHexString();
    }
    return out;
}

Byte &ByteBuffer::operator[](Index ind) {
    size_t size = this->size();
    if (ind < 0)
        ind += size;
    assert(0 <= ind && (unsigned long) ind < size);
    return bytes[ind];
}

std::vector<Byte> ByteBuffer::toVector() const {
    return bytes;
}

Byte *ByteBuffer::data() {
    return bytes.data();
}

ByteBuffer ByteBuffer::slice(ByteBuffer::Index start) {
    return this->slice(start, this->size());
}

ByteBuffer ByteBuffer::slice(ByteBuffer::Index start, ByteBuffer::Index end) {
    size_t size = this->size();
    // 处理负下标
    if (start < 0)
        start += size;
    assert(0 <= start && (unsigned long) start < size);
    if (end < 0)
        end += size;
    assert(0 < end && (unsigned long) end <= size);
    // 创建新缓冲区
    return ByteBuffer(std::vector<Byte>(bytes.begin() + start, bytes.begin() + end));
}

ByteBuffer &ByteBuffer::operator=(const std::vector<Byte> &buffer) {
    bytes = buffer;
    return *this;
}

bool operator==(const ByteBuffer &bufferA, const ByteBuffer &bufferB) {
    return bufferA.bytes == bufferB.bytes;
}
