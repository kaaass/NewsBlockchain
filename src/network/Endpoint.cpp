#include "Endpoint.h"

#include <utility>
#include <glog/logging.h>

using namespace std;
using namespace restbed;
using json = nlohmann::json;

std::vector<Endpoint *> Endpoint::POOL;

void Endpoint::setPath(std::string pPath) {
    path = std::move(pPath);
}

void Endpoint::setGetLogic(Endpoint::HandleLogic handleLogic) {
    logicGet = std::move(handleLogic);
}

void Endpoint::setPostLogic(Endpoint::HandleLogic handleLogic) {
    logicPost = std::move(handleLogic);
}

Endpoint Endpoint::httpGet(std::string path, Endpoint::HandleLogic handleLogic) {
    auto endpoint = Endpoint();
    endpoint.setPath(std::move(path));
    endpoint.setGetLogic(std::move(handleLogic));
    return endpoint;
}

Endpoint Endpoint::httpPost(std::string path, Endpoint::HandleLogic handleLogic) {
    auto endpoint = Endpoint();
    endpoint.setPath(std::move(path));
    endpoint.setPostLogic(std::move(handleLogic));
    return endpoint;
}

std::shared_ptr<restbed::Resource> Endpoint::buildResource() {
    auto resource = make_shared<Resource>();
    resource->set_path(path);
    auto clone = new Endpoint(*this);
    POOL.push_back(clone); // 加入Endpoint池
    if (logicGet != nullptr)
        resource->set_method_handler("GET", bind1st(mem_fun(&Endpoint::handleGetImpl), clone));
    if (logicPost != nullptr)
        resource->set_method_handler("POST", bind1st(mem_fun(&Endpoint::handlePostImpl), clone));
    return resource;
}

void Endpoint::handleGetImpl(std::shared_ptr<restbed::Session> origin) {
    JsonSession session(origin);
    JsonResponse response;
    // Get 的请求体为空
    // 调用处理逻辑
    json result;
    try {
        logicGet(session, response);
        // 返回 Json
        result["status"] = response.code == OK ? "OK" : "ERR";
        if (!response.message.empty())
            result["message"] = response.message;
        if (!response.data.empty())
            result["data"] = response.data;
    } catch (const exception &e) {
        result["status"] = "ERR";
        response.code = INTERNAL_SERVER_ERROR;
        result["message"] = e.what();
        LOG(WARNING) << "Unexpected exception: " << e.what();
    }
    auto jsonStr = result.dump();
    // 返回拼接
    ::Response sessionResp;
    sessionResp.set_status_code(response.code);
    sessionResp.set_body(jsonStr);
    sessionResp.add_header("Content-Length", ::to_string(jsonStr.size()));
    sessionResp.add_header("Content-Type", "application/json");
    origin->close(sessionResp);
}

void Endpoint::handlePostImpl(const std::shared_ptr<restbed::Session> origin) {
    auto request = origin->get_request();
    size_t content_length = request->get_header("Content-Length", 0);
    origin->fetch(content_length, [=](const shared_ptr<Session> origin, const Bytes &body) {
        JsonSession session(origin);
        JsonResponse response;
        json result;
        try {
            // 处理 Post 请求体
            json jsonBody = json::parse(string(body.begin(), body.end()));
            session.body = jsonBody;
            // 调用处理逻辑
            logicPost(session, response);
            // 返回 Json
            result["status"] = response.code == OK ? "OK" : "ERR";
            if (!response.message.empty())
                result["message"] = response.message;
            if (!response.data.empty())
                result["data"] = response.data;
        } catch (const json::exception &e) {
            result["status"] = "ERR";
            response.code = BAD_REQUEST;
            result["message"] = "请求体必须为Json格式！";
        } catch (const exception &e) {
            result["status"] = "ERR";
            response.code = INTERNAL_SERVER_ERROR;
            result["message"] = e.what();
            LOG(WARNING) << "Unexpected exception: " << e.what();
        }
        auto jsonStr = result.dump();
        // 返回拼接
        ::Response sessionResp;
        sessionResp.set_status_code(response.code);
        sessionResp.set_body(jsonStr);
        sessionResp.add_header("Content-Length", ::to_string(jsonStr.size()));
        sessionResp.add_header("Content-Type", "application/json");
        origin->close(sessionResp);
    });
}

void Endpoint::freePool() {
    for (auto ptr: POOL)
        delete ptr;
}
