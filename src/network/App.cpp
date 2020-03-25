#include "App.h"
#include <network/Endpoint.h>
#include <network/GlogLogger.h>
#include <string>

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
    Endpoint::httpGet("/test/{para: .*}", HANDLE_LOGIC(session, response) {
        string para = session.request->get_path_parameter("para");
        response.data["test"] = "233333";
        response.data["para"] = para;
    }).publish(service);
    Endpoint::httpPost("/post/", HANDLE_LOGIC(session, response) {
        auto &body = session.body;
        if (!body.contains("test")) {
            response.code = 400;
            response.message = "缺少参数test";
            return;
        }
        response.data["test"] = body["test"];
    }).publish(service);
}

void App::finalize() {
    // 释放端点对象
    Endpoint::freePool();
}
