#include <unistd.h>
#include <sys/wait.h>

#include <format>
#include <iostream>
#include <cstdlib>
#include <string_view>

#include "luanch.hpp"

extern char **environ;

bool check_environment_value(const char *name, std::string_view value);
bool add_environment_value(const char *name, std::string_view value);

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    if (!check_environment_value("LD_LIBRARY_PATH", ".")) {
        add_environment_value("LD_LIBRARY_PATH", ".");
        int pid = vfork(), stat;
        if (pid) {
            return wait(&stat) != pid;
        }
        int errc = execve(argv[0], const_cast<char *const *>(argv), environ);
        std::cout << std::format("启动失败, code: {}", errc);
    }
    return luanch_main("test_u8main", argc, argv);
}

bool check_environment_value(const char *name, std::string_view value)
{
#ifdef _WIN32
    constexpr auto splitChar = ";";
#else
    constexpr auto splitChar = ":";
#endif
    auto env = getenv(name);
    if (env) {
        auto strenv   = std::string(splitChar).append(env).append(splitChar);
        auto strvalue = std::string(splitChar).append(value).append(splitChar);
        return strenv.find(strvalue) != std::string::npos;
    }
    return false;
}

bool add_environment_value(const char *name, std::string_view value)
{
#ifdef _WIN32
    constexpr auto splitChar = ";";
#else
    constexpr auto splitChar = ":";
#endif
    auto env = getenv(name);
    if (env) {
        return setenv(name, std::string(env).append(splitChar).append(value).c_str(), 1) == 0;
    }
    return setenv(name, value.data(), 1) == 0;
}
