#ifndef _TEST_3RDWRAP_VULKAN_LIBRARY_H_
#define _TEST_3RDWRAP_VULKAN_LIBRARY_H_
#include <test/3rdwrap/3rdwrap_library.h>

#define TEST_VULKAN_BEGIN                                                                          \
    namespace vk                                                                                   \
    {
#define TEST_VULKAN_END }

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC true

#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_RAII_NO_EXCEPTIONS

#endif