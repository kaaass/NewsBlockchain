#include <string>
#include <memory>
#include <cstdlib>
#include <restbed>
#include <json.hpp>
#include <chrono>
#include <network/Endpoint.h>
#include <network/GlogLogger.h>

using namespace std;
using namespace restbed;
using json = nlohmann::json;

void get_method_handler(const shared_ptr<Session> session) {
    const auto &request = session->get_request();

    string name = request->get_path_parameter("name");
    json ret;
    auto dur = chrono::system_clock::now().time_since_epoch();
    ret["ts"] = chrono::duration_cast<chrono::seconds>(dur).count();
    ret["name"] = name;
    ret["message"] = "Hello, " + name;
    const string body = ret.dump();

    auto resp = new Response();
    resp->set_body(ret.dump());
    resp->set_status_code(OK);
    resp->add_header("X-Test", "test");
    session->close(*resp);
}

int main(const int, const char **argv) {
    // 日志配置
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    FLAGS_alsologtostderr = true;
    FLAGS_minloglevel = 0;

    auto resource = make_shared<Resource>();
    resource->set_path("/resource/{name: .*}");
    resource->set_method_handler("GET", get_method_handler);

    auto testResc =
            Endpoint::httpGet("/test/{para: .*}", HANDLE_LOGIC(session, response) {
                string para = session.request->get_path_parameter("para");
                response.data["test"] = "233333";
                response.data["para"] = para;
            }).buildResource();

    auto settings = make_shared<Settings>();
    settings->set_port(1984);
    settings->set_default_header("Connection", "close");

    Service service;
    service.publish(resource);
    service.publish(testResc);
    service.set_logger(make_shared<GlogLogger>());
    service.start(settings);

    return EXIT_SUCCESS;
}