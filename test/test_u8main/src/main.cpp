#include <test/config.h>

#include <spdlog/spdlog.h>

#include "luancher/main.hpp"
#include "luancher/main.cpp"

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    SPDLOG_INFO("hello world");
    return 0;
}
