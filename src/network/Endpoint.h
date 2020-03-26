#ifndef NEWS_BLOCKCHAIN_ENDPOINT_H
#define NEWS_BLOCKCHAIN_ENDPOINT_H

#include <map>
#include <string>
#include <functional>
#include <memory>
#include <restbed>
#include <json.hpp>

/**
 * RESTful 端点
 */
class Endpoint {

    /**
     * 端点对象池
     *
     * 用于保证端点函数在整个服务器生命周期存在
     */
    static std::vector<Endpoint *> POOL;

public:
    struct JsonSession {
        const std::shared_ptr<restbed::Session> &origin;
        std::shared_ptr<const restbed::Request> request;
        nlohmann::json body;

        explicit JsonSession(const std::shared_ptr<restbed::Session> &session)
                : origin(session), request(session->get_request()) {}
    };

    struct JsonResponse {
        /**
         * 返回码
         */
        int code = restbed::OK;

        /**
         * 错误信息
         */
        std::string message;

        /**
         * 返回数据
         */
        nlohmann::json data;

        JsonResponse() = default;
    };

    /**
     * 待处理逻辑类型
     */
    typedef std::function<void(const JsonSession &, JsonResponse &)> HandleLogic;

    Endpoint() = default;

private:
    std::string path;

    HandleLogic logicGet = nullptr;
    HandleLogic logicPost = nullptr;

public:

    void setPath(std::string pPath);

    void setGetLogic(HandleLogic handleLogic);

    void setPostLogic(HandleLogic handleLogic);

    static Endpoint httpGet(std::string path, HandleLogic handleLogic);

    static Endpoint httpPost(std::string path, HandleLogic handleLogic);

    std::shared_ptr<restbed::Resource> buildResource();

    void publish(restbed::Service &service);

    /**
     * 释放端点对象池
     */
    static void freePool();

private:

    void handleGetImpl(std::shared_ptr<restbed::Session> session);

    void handlePostImpl(std::shared_ptr<restbed::Session> session);
};

#define HANDLE_LOGIC(req, resp) [](const Endpoint::JsonSession &req, Endpoint::JsonResponse &resp)

#endif //NEWS_BLOCKCHAIN_ENDPOINT_H
