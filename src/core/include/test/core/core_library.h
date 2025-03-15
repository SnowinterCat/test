
#ifndef _TEST_CORE_LIBRARY_H_
#define _TEST_CORE_LIBRARY_H_

#if defined(_WIN32) && defined(TEST_CORE_DLL)
    #if defined(TEST_CORE_EXPORTS)
        #define TEST_CORE_API __declspec(dllexport)
    #else
        #define TEST_CORE_API __declspec(dllimport)
    #endif
#endif

#if !defined(TEST_CORE_API)
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define TEST_CORE_API __attribute__((visibility("default")))
    #else
        #define TEST_CORE_API
    #endif
#endif

#define TEST_BEGIN                                                                                 \
    namespace test                                                                                 \
    {
#define TEST_END }

#endif