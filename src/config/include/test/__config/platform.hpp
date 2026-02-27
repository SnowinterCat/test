#pragma once
#include <test/config.hpp>

// define TEST_FORCE_EXPORT_API
#if defined(_WIN32)
    #define TEST_FORCE_EXPORT_API __declspec(dllexport)
#endif
#if !defined(TEST_FORCE_EXPORT_API)
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_FORCE_EXPORT_API __attribute__((visibility("default")))
    #else
        #define TEST_FORCE_EXPORT_API
    #endif
#endif
