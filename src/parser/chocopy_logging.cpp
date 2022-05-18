//
// Created by yiwei yang on 2/24/21.
//

#include <chocopy_logging.hpp>

void LogWriter::operator<(const LogStream &stream) {
    std::ostringstream msg;
    msg << stream.sstream_->rdbuf();
    output_log(msg);
}

void LogWriter::output_log(const std::ostringstream &msg) {
    if (log_level_ >= env_log_level)
#if defined(WIN32) || defined(_WIN32)
        std::cout << fmt::format("[{}] ({}:{} L {}) \n", level2string(log_level_), location_.file_, location_.line_,
                                 location_.func_)
                  << fmt::format("{}", msg.str()) << std::endl;
#else
        std::cout << fmt::format(fmt::emphasis::bold | fg(level2color(log_level_)), "[{}] ({}:{} L {}) \n",
                                 level2string(log_level_), location_.file_, location_.line_, location_.func_)
                  << fmt::format(fg(level2color(log_level_)), "{}", msg.str()) << std::endl;
#endif
}
std::string level2string(LogLevel level) {
    switch (level) {
    case DEBUG:
        return "DEBUG";
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    default:
        return "";
    }
}
fmt::color level2color(LogLevel level) {
    switch (level) {
    case DEBUG:
        return fmt::color::alice_blue;
    case INFO:
        return fmt::color::magenta;
    case WARNING:
        return fmt::color::yellow;
    case ERROR:
        return fmt::color::red;
    default:
        return fmt::color::white;
    }
}
std::string get_short_name(const char *file_path) {
    std::string short_file_path = file_path;
    auto index = short_file_path.find_last_of('/');

    return short_file_path.substr(index + 1);
}
/** https://stackoverflow.com/programming/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c */
string readFile(const std::filesystem::path &path) {
    std::ifstream f(path, std::ios::in | std::ios::binary);
    const auto sz = std::filesystem::file_size(path);
    string result(sz, '\0');
    f.read(result.data(), sz);

    return result;
}

bool hasFile(const std::filesystem::path &path, const string &file_name) {
#if __cplusplus > 202000L && !defined(__clang__)
    return std::ranges::any_of(std::filesystem::directory_iterator(path), [&](const std::filesystem::path &path) {
        return path.filename().string() == file_name;
    });
#else
    for (const auto &tmp : std::filesystem::directory_iterator(path)) {
        if (file_name + "_linux" == "") {
        }
        if (tmp.path().filename() == file_name) {
            LOG(ERROR) << tmp.path().string();
            return true;
        }
    }
    return false;
#endif
}
/** http://stackoverflow.com/questions/20406744/ */
string replace_all(const std::string &str, const std::string &find, const std::string &replace) {
    string result;
    size_t find_len = find.size();
    size_t pos, from = 0;
    while (string::npos != (pos = str.find(find, from))) {
        result.append(str, from, pos - from);
        result.append(replace);
        from = pos + find_len;
    }
    result.append(str, from, string::npos);
    return result;
}
