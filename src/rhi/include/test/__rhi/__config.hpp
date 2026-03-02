#pragma once
#include <test/config.hpp>
// Standard Library
#include <version>

// TEST_RHI_API
#if defined(_WIN32) && defined(TEST_SHARED_BUILD)
    #if defined(TEST_RHI_COMPILING)
        #define TEST_RHI_API __declspec(dllexport)
    #else
        #define TEST_RHI_API __declspec(dllimport)
    #endif
#endif

#if !defined(TEST_RHI_API)
    #if !defined(TEST_NO_GCC_API_ATTRIBUTE) && defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_RHI_API __attribute__((visibility("default")))
    #else
        #define TEST_RHI_API
    #endif
#endif

#define TEST_RHI_BEGIN                                                                             \
    namespace rhi                                                                                  \
    {
#define TEST_RHI_END }

// clang-format off
#define TEST_RHI_LOG_TRACE(...)    SPDLOG_TRACE(__VA_ARGS__)
#define TEST_RHI_LOG_DEBUG(...)    SPDLOG_DEBUG(__VA_ARGS__)
#define TEST_RHI_LOG_INFO(...)     SPDLOG_INFO(__VA_ARGS__)
#define TEST_RHI_LOG_WARN(...)     SPDLOG_WARN(__VA_ARGS__)
#define TEST_RHI_LOG_ERROR(...)    SPDLOG_ERROR(__VA_ARGS__)
#define TEST_RHI_LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
// clang-format on

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC true
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_RAII_NO_EXCEPTIONS