#ifndef _TEST_3RDWRAP_LIBRARY_H_
#define _TEST_3RDWRAP_LIBRARY_H_
#include <test/config.h>

#if defined(_WIN32) && defined(TEST_3RDWRAP_DLL)
    #if defined(TEST_3RDWRAP_EXPORTS)
        #define TEST_3RDWRAP_API __declspec(dllexport)
    #else
        #define TEST_3RDWRAP_API __declspec(dllimport)
    #endif
#endif

#if !defined(TEST_3RDWRAP_API)
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_3RDWRAP_API __attribute__((visibility("default")))
    #else
        #define TEST_3RDWRAP_API
    #endif
#endif

#define TEST_3RDWRAP_BEGIN                                                                         \
    namespace wrap                                                                                 \
    {
#define TEST_3RDWRAP_END }

#endif