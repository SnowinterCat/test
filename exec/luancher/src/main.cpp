#include <test/config.h>

#if !defined(_WIN32)
    #include <unistd.h>
    #include <sys/wait.h>
#endif

#include <filesystem>
#include <iostream>
#include <format>

#include "luancher/main.hpp"
#include "luanch.hpp"

////////////////////////////////////////////////////////////////////////////////
// declaration
////////////////////////////////////////////////////////////////////////////////

#if !defined(_WIN32)
bool check_environment_value(const char *name, std::string_view value);
bool add_environment_value(const char *name, std::string_view value);
#endif

////////////////////////////////////////////////////////////////////////////////
// definition
////////////////////////////////////////////////////////////////////////////////

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
#if !defined(_WIN32)
    if (!check_environment_value("LD_LIBRARY_PATH", ".")) {
        add_environment_value("LD_LIBRARY_PATH", ".");
        int pid = vfork(), stat;
        if (pid) {
            return wait(&stat) != pid;
        }
        int errc = execve(argv[0], const_cast<char *const *>(argv), environ);
        std::cout << std::format("启动失败, code: {}", errc);
    }
#endif
    return luanch_main(std::filesystem::path(argv[0]).stem().string().c_str(), argc, argv);
}

#include "luancher/main.cpp"

#if !defined(_WIN32)
bool check_environment_value(const char *name, std::string_view value)
{
    auto env = getenv(name);
    if (env) {
        auto strenv   = std::string(":").append(env).append(":");
        auto strvalue = std::string(":").append(value).append(":");
        return strenv.find(strvalue) != std::string::npos;
    }
    return false;
}

bool add_environment_value(const char *name, std::string_view value)
{
    auto env = getenv(name);
    if (env) {
        return setenv(name, std::string(env).append(":").append(value).c_str(), 1) == 0;
    }
    return setenv(name, value.data(), 1) == 0;
}
#endif