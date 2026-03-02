#include <test/__rhi/functions.hpp>
// Standard Library
// System Library
// Third-Party Library
// Local Library

TEST_BEGIN
TEST_RHI_BEGIN

auto TEST_RHI_API createVkRaiiInstanceAndMessengerDefault(const ::vk::raii::Context   &context,
                                                          std::span<const char *const> extensions,
                                                          bool                         debug)
    -> std::tuple<::vk::Result, ::vk::raii::Instance, ::vk::raii::DebugUtilsMessengerEXT>
{
    auto layerProperties     = context.enumerateInstanceLayerProperties();
    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    ;
}

TEST_RHI_END
TEST_END