#include <cjson/cJSON.h>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    std::filesystem::path path = argv[1];
    std::ifstream f(path, std::ios::in | std::ios::binary);
    const auto sz = std::filesystem::file_size(path);
    std::string result(sz, '\0');
    f.read(result.data(), sz);
    f.close();
    auto rs = cJSON_Parse(result.c_str());
    std::cout << cJSON_Print(rs);
    return 0;
}
