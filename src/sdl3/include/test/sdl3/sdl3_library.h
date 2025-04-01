#ifndef _TEST_SDL3_LIBRARY_H_
#define _TEST_SDL3_LIBRARY_H_
#include <test/config.h>

#if defined(_WIN32) && defined(TEST_SDL3_DLL)
    #if defined(TEST_SDL3_EXPORTS)
        #define TEST_SDL3_API __declspec(dllexport)
    #else
        #define TEST_SDL3_API __declspec(dllimport)
    #endif
#endif

#if !defined(TEST_SDL3_API)
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_SDL3_API __attribute__((visibility("default")))
    #else
        #define TEST_SDL3_API
    #endif
#endif

#define TEST_SDL3_BEGIN                                                                            \
    namespace sdl3                                                                                 \
    {
#define TEST_SDL3_END }

#endif