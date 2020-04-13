#include <fstream>
#include <util/StringUtil.h>
#include <glog/logging.h>
#include "StaticController.h"

using namespace std;
using namespace restbed;

void StaticController::publish(restbed::Service &service) {
    auto indexResc = make_shared<Resource>();
    indexResc->set_path("/");
    indexResc->set_method_handler("GET", StaticController::indexHandler);

    auto staticResc = make_shared<Resource>();
    staticResc->set_paths({"/app/{filename: .*\\.html}",
                           "/app/js/{filename: .*\\.js}",
                           "/app/css/{filename: .*\\.css}",
                           "/app/templates/{filename: .*\\.hbs}"});
    staticResc->set_method_handler("GET", StaticController::staticHandler);

    service.publish(indexResc);
    service.publish(staticResc);
}

/**
 * 首页跳转处理
 */
void StaticController::indexHandler(const std::shared_ptr<restbed::Session> session) {
    const multimap<string, string> headers{
            {"Location", "/app/index.html"}
    };

    session->close(FOUND, "", headers);
}

/**
 * 静态资源获取处理
 */
void StaticController::staticHandler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    string filename = request->get_path_parameter("filename");

    // 简单防止路径穿透
    if (filename.find("..") != string::npos) {
        session->close(BAD_REQUEST);
        return;
    }

    // 匹配 MIME
    std::string mime;

    if (StringUtil::endsWith(filename, ".html")) {
        mime = "text/html";
    } else if (StringUtil::endsWith(filename, ".js")) {
        mime = "application/x-javascript";
        filename = "js/" + filename;
    } else if (StringUtil::endsWith(filename, ".css")) {
        mime = "text/css";
        filename = "css/" + filename;
    } else if (StringUtil::endsWith(filename, ".hbs")) {
        mime = "text/plain";
        filename = "templates/" + filename;
    } else {
        mime = "text/plain";
    }

    // 打开文件
    ifstream stream("./static/" + filename, ifstream::in);
    if (stream.is_open()) {
        const string body = string(istreambuf_iterator<char>(stream), istreambuf_iterator<char>());
        const multimap<string, string> headers{
                {"Content-Type",   mime},
                {"Content-Length", ::to_string(body.length())}
        };

        session->close(OK, body, headers);
    } else {
        LOG(INFO) << "Cannot find static file: " << filename;
        session->close(NOT_FOUND);
    }
}
