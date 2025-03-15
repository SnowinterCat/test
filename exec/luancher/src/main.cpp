#include <test/config.h>

#include <filesystem>

#include "luancher/main.hpp"
#include "luancher/main.cpp"
#include "luanch.hpp"

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    return luanch_main(std::filesystem::path(argv[0]).stem().string().c_str(), argc, argv);
}
