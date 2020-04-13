#ifndef NEWS_BLOCKCHAIN_GLOGLOGGER_H
#define NEWS_BLOCKCHAIN_GLOGLOGGER_H

#include <restbed>
#include <glog/logging.h>
#include <string>
#include <vector>

class GlogLogger : public restbed::Logger {

    char strbuf[100];
    std::vector<std::string> levelMsg = {
            std::string("INFO"), // 0000
            std::string("DEBUG"), // 1000
            std::string("FATAL"), // 2000
            std::string("ERROR"), // 3000
            std::string("WARNING"), // 4000
            std::string("SECURITY"), // 5000
    };

public:
    void stop() override {
    }

    void start(const std::shared_ptr<const restbed::Settings> &) override {
    }

    void log(const Level level, const char *format, ...) override {
        va_list arguments;
        va_start(arguments, format);

        vsprintf(strbuf, format, arguments);

        switch (level) {
            case DEBUG:
                DLOG(INFO) << "RestbedLog[" << levelMsg[level] << "] " << strbuf;
                break;
            case ERROR:
            case WARNING:
            case SECURITY:
                LOG(WARNING) << "RestbedLog[" << levelMsg[level] << "] " << strbuf;
                break;
            case FATAL:
                LOG(FATAL) << "RestbedLog[" << levelMsg[level] << "] " << strbuf;
                break;
            case INFO:
            default:
                LOG(INFO) << "RestbedLog[" << levelMsg[0] << "] " << strbuf;
        }

        va_end(arguments);
    }

    void log_if(bool expression, const Level level, const char *format, ...) override {
        if (expression) {
            va_list arguments;
            va_start(arguments, format);
            log(level, format, arguments);
            va_end(arguments);
        }
    }
};

#endif //NEWS_BLOCKCHAIN_GLOGLOGGER_H
