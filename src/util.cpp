#include <memory>
#include <array>
#include "util.h"

std::string strip_tail_endline(std::string dir)
{
    std::string result=dir;
    result = (result.back() == '\r')? result.substr(0, result.length()-1) : result;
    result = (result.back() == '\n')? result.substr(0, result.length()-1) : result;
    return result;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

