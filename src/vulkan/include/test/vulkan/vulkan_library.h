#ifndef _TEST_VULKAN_LIBRARY_H_
#define _TEST_VULKAN_LIBRARY_H_
#include <test/config.h>

#if defined(_WIN32) && defined(TEST_VULKAN_DLL)
    #if defined(TEST_VULKAN_EXPORTS)
        #define TEST_VULKAN_API __declspec(dllexport)
    #else
        #define TEST_VULKAN_API __declspec(dllimport)
    #endif
#endif

#if !defined(TEST_VULKAN_API)
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_VULKAN_API __attribute__((visibility("default")))
    #else
        #define TEST_VULKAN_API
    #endif
#endif

#define TEST_VULKAN_BEGIN                                                                          \
    namespace vk                                                                                   \
    {
#define TEST_VULKAN_END }

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC true

#endif