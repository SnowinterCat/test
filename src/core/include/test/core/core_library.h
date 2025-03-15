
#ifndef _TEST_BASE_LIBRARY_H_
#define _TEST_BASE_LIBRARY_H_

#if defined(_WIN32) && defined(TEST_BASE_DLL)
    #if defined(TEST_BASE_EXPORTS)
        #define TEST_BASE_API __declspec(dllexport)
    #else
        #define TEST_BASE_API __declspec(dllimport)
    #endif
#endif

#if !defined(TEST_BASE_API)
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_BASE_API __attribute__((visibility("default")))
    #else
        #define TEST_BASE_API
    #endif
#endif

#define TEST_BEGIN                                                                                 \
    namespace test                                                                                 \
    {
#define TEST_END }

#endif