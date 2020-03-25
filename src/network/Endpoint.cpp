#include "Endpoint.h"

#include <utility>

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
        resource->set_method_handler("POST", bind1st(mem_fun(&Endpoint::handleGetImpl), clone));
    return resource;
}

void Endpoint::handleGetImpl(std::shared_ptr<restbed::Session> session) {
    JsonSession request(session);
    JsonResponse response;
    // Get 的请求体为空
    // 调用处理逻辑
    logicGet(request, response);
    // 返回 Json
    json result;
    result["status"] = response.code == OK ? "OK" : "ERR";
    result["data"] = response.data;
    auto jsonStr = result.dump();
    // 返回拼接
    ::Response sessionResp;
    sessionResp.set_status_code(response.code);
    sessionResp.set_body(jsonStr);
    sessionResp.add_header("Content-Length", ::to_string(jsonStr.size()));
    sessionResp.add_header("Content-Type", "application/json");
    session->close(sessionResp);
}

void Endpoint::handlePostImpl(const std::shared_ptr<restbed::Session> session) {
    JsonSession request(session);
    JsonResponse response;
    // TODO: 处理Post请求体
    // 调用处理逻辑
    logicGet(request, response);
    // 返回 Json
    json result;
    result["status"] = response.code == OK ? "OK" : "ERR";
    result["data"] = response.data;
    auto jsonStr = result.dump();
    // 返回拼接
    ::Response sessionResp;
    sessionResp.set_status_code(response.code);
    sessionResp.set_body(jsonStr);
    sessionResp.add_header("Content-Length", ::to_string(jsonStr.size()));
    sessionResp.add_header("Content-Type", "application/json");
    session->close(sessionResp);
}

void Endpoint::freePool() {
    for (auto ptr: POOL)
        delete ptr;
}
