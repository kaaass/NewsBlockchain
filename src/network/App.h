#ifndef NEWS_BLOCKCHAIN_APP_H
#define NEWS_BLOCKCHAIN_APP_H

#include <restbed>
#include <memory>

/**
 * 新闻区块链App
 */
class App {
    int port;

    restbed::Service service;
    std::shared_ptr<restbed::Settings> settings;

    void prepareSettings();

    void registerController();

public:

    explicit App(int port) : port(port) {}

    /**
     * 初始化App
     */
    void initialize();

    /**
     * 启动App
     */
    void start();

    /**
     * 结束App
     */
    void finalize();
};


#endif //NEWS_BLOCKCHAIN_APP_H
