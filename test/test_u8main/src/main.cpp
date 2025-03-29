#include <test/config.h>
#include "luancher/u8main.hpp"

#include <spdlog/spdlog.h>

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    SPDLOG_INFO("hello world");
    return 0;
}
