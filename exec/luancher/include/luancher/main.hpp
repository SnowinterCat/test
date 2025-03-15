#pragma once

#include <test/config.h>

#if defined(_WIN32)
    #define TEST_EXPORT_API __declspec(dllexport)
#endif
#if !defined(TEST_EXPORT_API)
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_EXPORT_API __attribute__((visibility("default")))
    #else
        #define TEST_EXPORT_API
    #endif
#endif

#if defined(TEST_USE_LUANCHER) && TEST_USE_LUANCHER && !TEST_LUANCHER_EXPORTS
extern "C" TEST_EXPORT_API int u8main(int argc, const char *const *argv);
#else
extern "C" int u8main(int argc, const char *const *argv);
#endif
