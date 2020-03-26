#ifndef NEWS_BLOCKCHAIN_CONTROLLER_H
#define NEWS_BLOCKCHAIN_CONTROLLER_H

#include <restbed>

/**
 * 区块链查看控制器
 *
 * /api/block
 */
class BlockController {
public:
    static void publish(restbed::Service &service);
};

/**
 * 哈希控制器
 *
 * /api/hash
 */
class HashController {
public:
    static void publish(restbed::Service &service);
};

/**
 * Huffman控制器
 *
 * /api/huffman
 */
class HuffmanController {
public:
    static void publish(restbed::Service &service);
};

/**
 * 校验控制器
 *
 * /api/validate
 */
class ValidateController {
public:
    static void publish(restbed::Service &service);
};

/**
 * 搜索控制器
 *
 * /api/search
 */
class SearchController {
public:
    static void publish(restbed::Service &service);
};


#endif //NEWS_BLOCKCHAIN_CONTROLLER_H
