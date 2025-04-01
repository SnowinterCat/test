#include <test/config.h>
#include "luancher/u8main.hpp"

#include <spdlog/spdlog.h>
#include <test/vulkan.hpp>

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    SPDLOG_INFO("hello vulkan");

    vk::raii::Context context;

    context.enumerateInstanceVersion();
    context.enumerateInstanceLayerProperties();
    context.enumerateInstanceExtensionProperties();
    context.createInstance();

    return 0;
}
