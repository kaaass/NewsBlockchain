#include "StringUtil.h"

std::vector<ByteBuffer> StringUtil::splitParagraph(const std::string &str) {
    std::vector<ByteBuffer> output;
    size_t first = 0;
    while (first < str.size()) {
        const auto second = str.find_first_of('\n', first);
        if (first != second) {
            auto cur = str.substr(first, second - first);
            output.emplace_back((void *) cur.data(), cur.size());
        }
        if (second == std::string::npos)
            break;
        first = second + 1;
    }
    return output;
}

/*
 * Code from: http://quick-bench.com/mhyUI8Swxu3As-RafVUSVfEZd64
 */
std::vector<std::string> StringUtil::split(const std::string &str, const std::string &delims = " ") {
    std::vector<std::string> output;
    size_t first = 0;
    while (first < str.size()) {
        const auto second = str.find_first_of(delims, first);
        if (first != second) {
            output.emplace_back(str.substr(first, second - first));
        }
        if (second == std::string::npos)
            break;
        first = second + 1;
    }
    return output;
}
