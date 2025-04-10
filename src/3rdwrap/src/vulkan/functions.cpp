#include "test/3rdwrap/vulkan/functions.hpp"

#include <spdlog/spdlog.h>

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_VULKAN_BEGIN

auto TEST_3RDWRAP_API CreateDefaultVkInstance(const ::vk::raii::Context   &context,
                                              std::span<const char *const> extensions, bool isDebug)
    -> std::expected<Instance, ::vk::Result>
{
    Instance instance(nullptr, nullptr);

    auto layerProperties     = context.enumerateInstanceLayerProperties();
    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    auto enabledLayers       = std::vector<const char *>();
    auto enabledExtensions   = std::vector<const char *>(extensions.begin(), extensions.end());

    auto appInfo =
        ::vk::ApplicationInfo("",
                              ::vk::makeApiVersion(TEST_VERSION_MAJOR, TEST_VERSION_MINOR,
                                                   TEST_VERSION_ALTER, TEST_VERSION_BUILD),
                              "No Engine",
                              ::vk::makeApiVersion(TEST_VERSION_MAJOR, TEST_VERSION_MINOR,
                                                   TEST_VERSION_ALTER, TEST_VERSION_BUILD),
                              ::vk::ApiVersion12);

    // create ::vk::raii::instance
    ::vk::DebugUtilsMessengerCreateInfoEXT        messengerInfo;
    std::vector<::vk::ValidationFeatureEnableEXT> vdFeatureEnable;
    ::vk::ValidationFeaturesEXT                   vdFeatures;
    if (isDebug) {
        if (IsLayerAvailable(layerProperties, "VK_LAYER_KHRONOS_validation")) {
            enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
        }
        if (IsLayerAvailable(layerProperties, "VK_LAYER_LUNARG_monitor")) {
            enabledLayers.push_back("VK_LAYER_LUNARG_monitor");
        }
        if (IsExtensionAvailable(extensionProperties, VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
            enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        messengerInfo = ::vk::DebugUtilsMessengerCreateInfoEXT(
            {},
            ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            ::vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                ::vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                ::vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            &DefaultDebugMessengerCallback, nullptr);
        vdFeatureEnable = {::vk::ValidationFeatureEnableEXT::eDebugPrintf};
        vdFeatures      = ::vk::ValidationFeaturesEXT(vdFeatureEnable, {}, &messengerInfo);
    }
    auto instanceInfo = ::vk::InstanceCreateInfo({}, &appInfo, enabledLayers, enabledExtensions,
                                                 isDebug ? &vdFeatures : nullptr);
    if (auto res = context.createInstance(instanceInfo); res)
        instance.instance = std::move(res.value());
    else {
        return std::unexpected(res.error());
    }

    // create ::vk::raii::DebugUtilsMessengerEXT
    if (isDebug) {
        if (auto res = instance.instance.createDebugUtilsMessengerEXT(messengerInfo); res)
            instance.messenger = std::move(res.value());
        else {
            return std::unexpected(res.error());
        }
    }
    return instance;
}

//

bool IsLayerAvailable(const std::vector<::vk::LayerProperties> &properties, const char *layer)
{
    for (const auto &p : properties)
        if (strcmp(p.layerName, layer) == 0)
            return true;
    return false;
}

bool IsExtensionAvailable(const std::vector<::vk::ExtensionProperties> &properties,
                          const char                                   *extension)
{
    for (const auto &p : properties)
        if (strcmp(p.extensionName, extension) == 0)
            return true;
    return false;
}

//

VKAPI_ATTR VkBool32 VKAPI_CALL DefaultDebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/)
{
    if (static_cast<uint32_t>(pCallbackData->messageIdNumber) == 0x822806fa) {
        // Validation Warning: vkCreateInstance(): to enable extension VK_EXT_debug_utils,
        // but this extension is intended to support use by applications when
        // debugging and it is strongly recommended that it be otherwise avoided.
        return ::vk::False;
    }
    else if (static_cast<uint32_t>(pCallbackData->messageIdNumber) == 0xe8d1a9fe) {
        // Validation Performance Warning: Using debug builds of the validation layers *will*
        // adversely affect performance.
        return ::vk::False;
    }
    if (static_cast<::vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity) ==
            ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo &&
        strcmp(pCallbackData->pMessageIdName, "WARNING-DEBUG-PRINTF") != 0) {
        // 忽略 info 等级中，除了 vk::ValidationFeatureEnableEXT::eDebugPrintf 的所有消息
        return ::vk::False;
    }

    auto tmp = fmt::format(
        "\n{}: {}:\n\tmessageIDName   = <{}>\n\tmessageIdNumber = {}\n\tmessage         = <{}>",
        ::vk::to_string(static_cast<::vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)),
        ::vk::to_string(static_cast<::vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)),
        pCallbackData->pMessageIdName, pCallbackData->messageIdNumber, pCallbackData->pMessage);
    if (0 < pCallbackData->queueLabelCount) {
        tmp = fmt::format("{}\n\tQueue Labels:", tmp);
        for (uint32_t i = 0; i < pCallbackData->queueLabelCount; ++i) {
            tmp = fmt::format("{}\n\t\tlabelName = <{}>", tmp,
                              pCallbackData->pQueueLabels[i].pLabelName);
        }
    }
    if (0 < pCallbackData->cmdBufLabelCount) {
        tmp = fmt::format("{}\n\tCommandBuffer Labels:", tmp);
        for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
            tmp = fmt::format("{}\n\t\tlabelName = <{}>", tmp,
                              pCallbackData->pCmdBufLabels[i].pLabelName);
        }
    }
    if (0 < pCallbackData->objectCount) {
        tmp = fmt::format("{}\n\tObjects:", tmp);
        for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
            tmp = fmt::format("{}\n\t\tObject {}\n\t\t\tobjectType   = {}\n\t\t\tobjectHandle = {}",
                              tmp, i,
                              ::vk::to_string(static_cast<::vk::ObjectType>(
                                  pCallbackData->pObjects[i].objectType)),
                              pCallbackData->pObjects[i].objectHandle);
            if (pCallbackData->pObjects[i].pObjectName) {
                tmp = fmt::format("{}\n\t\t\tobjectName   = <{}>", tmp,
                                  pCallbackData->pObjects[i].pObjectName);
            }
        }
    }

    auto logLevel = spdlog::level::level_enum();
    switch (static_cast<::vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) {
    case ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
        logLevel = spdlog::level::debug;
        break;
    case ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
        logLevel = spdlog::level::info;
        break;
    case ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
        logLevel = spdlog::level::warn;
        break;
    case ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
        logLevel = spdlog::level::err;
        break;
    }
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), logLevel, "{}", tmp);
    return ::vk::False;
}

TEST_VULKAN_END
TEST_3RDWRAP_END
TEST_END