#include "test/3rdwrap/vulkan/functions.hpp"

#include <algorithm>
#include <limits>

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
    if (auto res = context.createInstance(instanceInfo); !res)
        return std::unexpected(res.error());
    else {
        instance.instance = std::move(res.value());
    }

    // create ::vk::raii::DebugUtilsMessengerEXT
    if (isDebug) {
        if (auto res = instance.instance.createDebugUtilsMessengerEXT(messengerInfo); !res)
            return std::unexpected(res.error());
        else {
            instance.messenger = std::move(res.value());
        }
    }
    return instance;
}

auto TEST_3RDWRAP_API CreateDefaultDevice(const Instance                     &instance,
                                          const ::vk::PhysicalDeviceFeatures &features)
    -> std::expected<Device, ::vk::Result>
{
    auto  device = Device();
    auto &inst   = instance.instance;

    auto &physicalDevGroup = device.physicalDeviceGroup;
    auto &physicalDev      = device.physicalDevice;
    auto &dev              = device.device;

    auto deviceGroups = inst.enumeratePhysicalDeviceGroups();
    if (deviceGroups.empty()) {
        return std::unexpected(::vk::Result::eErrorDeviceLost);
    }

    // physical device
    physicalDevGroup = SelectPhysicalDeviceGroup(deviceGroups, inst);
    physicalDev      = ::vk::raii::PhysicalDevice(inst, physicalDevGroup.physicalDevices[0]);

    // device queue
    auto queueProperties      = physicalDev.getQueueFamilyProperties();
    device.graphicsQueueIndex = SelectDeviceQueueFamilyIndex(
        queueProperties, ::vk::QueueFlagBits::eGraphics, ::vk::QueueFlags());
    device.transferQueueIndex = SelectDeviceQueueFamilyIndex(
        queueProperties, ::vk::QueueFlagBits::eTransfer, ::vk::QueueFlagBits::eGraphics);
    device.computeQueueIndex = SelectDeviceQueueFamilyIndex(
        queueProperties, ::vk::QueueFlagBits::eCompute, ::vk::QueueFlagBits::eGraphics);
    if (device.graphicsQueueIndex == std::numeric_limits<size_t>::max()) {
        return std::unexpected(::vk::Result::eErrorDeviceLost);
    }
    if (device.transferQueueIndex == std::numeric_limits<size_t>::max()) {
        device.transferQueueIndex = device.graphicsQueueIndex;
    }
    if (device.computeQueueIndex == std::numeric_limits<size_t>::max()) {
        device.computeQueueIndex = device.graphicsQueueIndex;
    }
    // device.graphicsQueueIndex = device.transferQueueIndex;
    // device.computeQueueIndex  = device.transferQueueIndex;
    SelectDeviceQueueIndex(
        queueProperties,
        std::span<uint64_t *const>(
            {&device.graphicsQueueIndex, &device.transferQueueIndex, &device.computeQueueIndex}));
    uint64_t queueIndex[]      = {device.graphicsQueueIndex, device.transferQueueIndex,
                                  device.computeQueueIndex};
    float    queuePriorities[] = {1.0, 0.5, 0.5};
    auto     queueInfos =
        BuildDeviceQueueCreateInfo({}, std::span(queueIndex), std::span(queuePriorities));
    // for (const auto &queueProperty : queueProperties) {
    //     SPDLOG_INFO("cnt: {}, flags: {}", queueProperty.queueCount,
    //                 ::vk::to_string(queueProperty.queueFlags));
    // }
    // SPDLOG_INFO("graphics: {}, transfer: {}, compute: {}", queueIndex[0], queueIndex[1],
    //             queueIndex[2]);
    // for (const auto &queueInfo : queueInfos) {
    //     auto tmp = fmt::format("index: {}, cnt: {}, priorities:", queueInfo.queueFamilyIndex,
    //                            queueInfo.queueCount);
    //     for (size_t i = 0; i < queueInfo.queueCount; ++i) {
    //         tmp = fmt::format("{} {}", tmp, queueInfo.pQueuePriorities[i]);
    //     }
    //     SPDLOG_INFO(tmp);
    // }

    // layers and extensions
    // Device layers is deprecated!!!!!!!!!!
    // See: https://github.com/KhronosGroup/Vulkan-Loader/issues/1086
    auto layerProperties = physicalDev.enumerateDeviceLayerProperties();
    auto extenProperties = physicalDev.enumerateDeviceExtensionProperties();
    auto enabledLayers   = std::vector<const char *>();
    auto enabledExtens   = std::vector<const char *>();
    // for (const auto &layer : layerProperties) {
    //     SPDLOG_INFO("name: {}, des: {}", layer.layerName.data(), layer.description.data());
    // }
    // SPDLOG_INFO("");
    // for (const auto &exten : extenProperties) {
    //     SPDLOG_INFO("name: {}", exten.extensionName.data());
    // }
    // SPDLOG_INFO("");
    if (IsExtensionAvailable(extenProperties, VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
        enabledExtens.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }
    // for (const auto &layer : enabledLayers) {
    //     SPDLOG_INFO("name: {}", layer);
    // }
    // SPDLOG_INFO("");
    // for (const auto &exten : enabledExtens) {
    //     SPDLOG_INFO("name: {}", exten);
    // }
    // SPDLOG_INFO("");

    // features

    // VkDeviceGroupDeviceCreateInfo
    auto devGroupInfo = ::vk::DeviceGroupDeviceCreateInfo(physicalDevGroup.physicalDeviceCount,
                                                          physicalDevGroup.physicalDevices);

    // create
    auto info = ::vk::DeviceCreateInfo({}, queueInfos, enabledLayers, enabledExtens, &features,
                                       &devGroupInfo);
    if (auto res = physicalDev.createDevice(info); !res)
        return std::unexpected(res.error());
    else {
        dev = std::move(res.value());
        device.graphicsQueue =
            dev.getQueue(device.graphicsQueueIndex & 0xFFFFFFFF, device.graphicsQueueIndex >> 32)
                .value();
        device.transferQueue =
            dev.getQueue(device.transferQueueIndex & 0xFFFFFFFF, device.transferQueueIndex >> 32)
                .value();
        device.computeQueue =
            dev.getQueue(device.computeQueueIndex & 0xFFFFFFFF, device.computeQueueIndex >> 32)
                .value();
    }
    return device;
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

auto SelectPhysicalDeviceGroup(const std::vector<::vk::PhysicalDeviceGroupProperties> &properties,
                               const ::vk::raii::Instance                             &instance)
    -> ::vk::PhysicalDeviceGroupProperties
{
    if (properties.empty()) {
        return ::vk::PhysicalDeviceGroupProperties();
    }

    for (const auto &groupProperty : properties) {
        auto property = groupProperty.physicalDevices[0].getProperties(*instance.getDispatcher());
        if (property.deviceType == ::vk::PhysicalDeviceType::eDiscreteGpu) {
            return groupProperty;
        }
    }
    for (const auto &groupProperty : properties) {
        auto property = groupProperty.physicalDevices[0].getProperties(*instance.getDispatcher());
        if (property.deviceType == ::vk::PhysicalDeviceType::eIntegratedGpu) {
            return groupProperty;
        }
    }
    for (const auto &groupProperty : properties) {
        auto property = groupProperty.physicalDevices[0].getProperties(*instance.getDispatcher());
        if (property.deviceType == ::vk::PhysicalDeviceType::eCpu) {
            return groupProperty;
        }
    }
    return properties[0];
}

auto SelectDeviceQueueFamilyIndex(const std::vector<::vk::QueueFamilyProperties> &properties,
                                  ::vk::QueueFlags required, ::vk::QueueFlags refused) -> size_t
{
    for (size_t i = 0; i < properties.size(); ++i) {
        if (properties[i].queueFlags & required && !(properties[i].queueFlags & refused))
            return i;
    }
    return std::numeric_limits<size_t>::max();
}

void SelectDeviceQueueIndex(const std::vector<::vk::QueueFamilyProperties> &properties,
                            std::span<uint64_t *const>                      queueIndices)
{
    auto cnt = std::vector<uint32_t>(properties.size());
    for (size_t i = 0; i < properties.size(); ++i) {
        cnt[i] = properties[i].queueCount;
    }

    for (size_t i = 0; i < queueIndices.size(); ++i) {
        assert(queueIndices[i]);
        uint64_t lowInt = *queueIndices[i] & 0xFFFFFFFF;
        assert(lowInt < properties.size());
        uint64_t highInt = properties[lowInt].queueCount - cnt[lowInt];
        if (cnt[lowInt] > 1)
            cnt[lowInt]--;
        *queueIndices[i] = (highInt << 32) | lowInt;
    }
}

auto BuildDeviceQueueCreateInfo(::vk::DeviceQueueCreateFlags deviceQueueCreateFlag,
                                std::span<uint64_t> queueIndices, std::span<float> queuePriorities)
    -> std::vector<::vk::DeviceQueueCreateInfo>
{
    assert(queueIndices.size() == queuePriorities.size());
    auto vec = std::vector<std::pair<uint64_t, float>>(queueIndices.size());
    for (size_t i = 0; i < queueIndices.size(); ++i) {
        vec[i].first  = queueIndices[i] >> 32 | (queueIndices[i] & 0xFFFFFFFF) << 32;
        vec[i].second = queuePriorities[i];
    }
    sort(vec.begin(), vec.end());
    for (size_t i = 0; i < vec.size(); ++i) {
        queueIndices[i]    = vec[i].first >> 32 | (vec[i].first & 0xFFFFFFFF) << 32;
        queuePriorities[i] = vec[i].second;
    }

    auto   info = std::vector<::vk::DeviceQueueCreateInfo>();
    size_t last = 0;
    for (size_t i = 1; i < vec.size(); ++i) {
        if ((queueIndices[i] & 0xFFFFFFFF) == (queueIndices[i - 1] & 0xFFFFFFFF))
            continue;
        info.push_back(::vk::DeviceQueueCreateInfo(
            deviceQueueCreateFlag, queueIndices[i - 1] & 0xFFFFFFFF,
            (queueIndices[i - 1] >> 32) - (queueIndices[last] >> 32) + 1, &queuePriorities[last]));
        last = i;
    }
    info.push_back(::vk::DeviceQueueCreateInfo(
        deviceQueueCreateFlag, queueIndices[queueIndices.size() - 1] & 0xFFFFFFFF,
        (queueIndices[queueIndices.size() - 1] >> 32) - (queueIndices[last] >> 32) + 1,
        &queuePriorities[last]));
    return info;
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