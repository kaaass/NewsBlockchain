#include <cstdlib>
#include <network/App.h>
#include <glog/logging.h>

int main(const int, const char **argv) {
    // 日志配置
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    FLAGS_alsologtostderr = true;
    FLAGS_minloglevel = 0;

    // 启动 App
    App app(8080);
    app.initialize();
    app.start();
    app.finalize();

    return EXIT_SUCCESS;
}