#pragma once
#include <test/config.hpp>
// Standard Library
#include <version>
// System Library
// Third-Party Library
// Local Library

// TEST_WSI_API
#if defined(_WIN32) && defined(TEST_SHARED_BUILD)
    #if defined(TEST_WSI_COMPILING)
        #define TEST_WSI_API __declspec(dllexport)
    #else
        #define TEST_WSI_API __declspec(dllimport)
    #endif
#endif

#if !defined(TEST_WSI_API)
    #if !defined(TEST_NO_GCC_API_ATTRIBUTE) && defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_WSI_API __attribute__((visibility("default")))
    #else
        #define TEST_WSI_API
    #endif
#endif

#define TEST_WSI_BEGIN                                                                             \
    namespace wsi                                                                                  \
    {
#define TEST_WSI_END }

// clang-format off
#define TEST_WSI_LOG_TRACE(...)    SPDLOG_TRACE(__VA_ARGS__)
#define TEST_WSI_LOG_DEBUG(...)    SPDLOG_DEBUG(__VA_ARGS__)
#define TEST_WSI_LOG_INFO(...)     SPDLOG_INFO(__VA_ARGS__)
#define TEST_WSI_LOG_WARN(...)     SPDLOG_WARN(__VA_ARGS__)
#define TEST_WSI_LOG_ERROR(...)    SPDLOG_ERROR(__VA_ARGS__)
#define TEST_WSI_LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
// clang-format on
