#include <string>
#include <memory>
#include <cstdlib>
#include <restbed>
#include <json.hpp>
#include <chrono>

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
    session->close(OK, body, {{"Content-Length", ::to_string(body.size())}});
}

int main(const int, const char **) {
    auto resource = make_shared<Resource>();
    resource->set_path("/resource/{name: .*}");
    resource->set_method_handler("GET", get_method_handler);

    auto settings = make_shared<Settings>();
    settings->set_port(1984);
    settings->set_default_header("Connection", "close");

    Service service;
    service.publish(resource);
    service.start(settings);

    return EXIT_SUCCESS;
}