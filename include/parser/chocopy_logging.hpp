//
// Created by yiwei yang on 2/24/21.
//

#ifndef CHOCOPY_COMPILER_CHOCOPY_LOGGING_HPP
#define CHOCOPY_COMPILER_CHOCOPY_LOGGING_HPP

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

using std::list;
using std::string;
enum LogLevel { DEBUG = 0, INFO, WARNING, ERROR };
struct LocationInfo {
    LocationInfo(string file, int line, const char *func) : file_(std::move(file)), line_(line), func_(func) {}
    ~LocationInfo() = default;

    string file_;
    int line_;
    const char *func_;
};
class LogStream;
class LogWriter;

class LogWriter {
public:
    LogWriter(const LocationInfo &location, LogLevel loglevel) : location_(location), log_level_(loglevel) {
        char *logv = std::getenv("LOGV");
        if (logv) {
            string string_logv = logv;
            env_log_level = std::stoi(logv);
        } else {
            env_log_level = 4;
        }
    };

    void operator<(const LogStream &stream);

private:
    void output_log(const std::ostringstream &g);
    LocationInfo location_;
    LogLevel log_level_;
    int env_log_level;
};

class LogStream {
public:
    LogStream() { sstream_ = new std::stringstream(); }
    ~LogStream() = default;

    template <typename T> LogStream &operator<<(const T &val) noexcept {
        (*sstream_) << val;
        return *this;
    }

    friend class LogWriter;

private:
    std::stringstream *sstream_;
};

string level2string(LogLevel level);
fmt::color level2color(LogLevel level);
string get_short_name(const char *file_path);
string readFile(const std::filesystem::path &path);
bool hasFile(const std::filesystem::path &path, const string &file_name);
string replace_all(const std::string &str, const std::string &find, const std::string &replace);
template <typename E> constexpr auto get_underlying(E e) noexcept { return static_cast<std::underlying_type_t<E>>(e); }

#define __FILESHORTNAME__ get_short_name(__FILE__)
#define LOG_IF(level) LogWriter(LocationInfo(__FILESHORTNAME__, __LINE__, __FUNCTION__), level) < LogStream()
#define LOG(level) LOG_##level
#define LOG_DEBUG LOG_IF(DEBUG)
#define LOG_INFO LOG_IF(INFO)
#define LOG_WARNING LOG_IF(WARNING)
#define LOG_ERROR LOG_IF(ERROR)

#endif // CHOCOPY_COMPILER_CHOCOPY_LOGGING_HPP
