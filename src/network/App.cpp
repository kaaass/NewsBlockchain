#include "App.h"
#include "Controller.h"
#include <network/Endpoint.h>
#include <network/GlogLogger.h>
#include <network/Serializer.h>
#include <blockchain/Blockchain.h>

using namespace std;
using namespace restbed;
using json = nlohmann::json;

void App::start() {
    LOG(INFO) << "Start web server at port: " << port;
    service.start(settings);
}

void App::initialize() {
    prepareSettings();
    registerController();
}

void App::prepareSettings() {
    // 配置服务器设置
    settings = make_shared<Settings>();
    settings->set_port(port);
    settings->set_default_header("Connection", "close");
    // 配置默认日志器
    service.set_logger(make_shared<GlogLogger>());
}

void App::registerController() {
    // 控制器：/api/block
    BlockController::publish(service);
}

void App::finalize() {
    // 释放端点对象
    Endpoint::freePool();
}
