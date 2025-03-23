#include <test/config.h>

#include <filesystem>

#include "luancher/main.hpp"
#include "luanch.hpp"

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