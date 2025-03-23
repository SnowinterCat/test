#include <test/config.h>
#include "luancher/main.hpp"

#if defined(_WIN32)
    #include <Windows.h>
#endif

#include <clocale>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// declaration
////////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
auto wargv_convert(int argc, const wchar_t *const *wargv, std::string &args,
                   std::vector<const char *> &argvector) -> const char *const *;
#endif

////////////////////////////////////////////////////////////////////////////////
// definition (header)
////////////////////////////////////////////////////////////////////////////////

#if defined(TEST_USE_LUANCHER) && TEST_USE_LUANCHER && !TEST_LUANCHER_EXPORTS
#else
    #if defined(_WIN32)
int wmain(int argc, const wchar_t *const *wargv)
    #else
int main(int argc, const char *const *argv)
    #endif
{
    std::setlocale(LC_ALL, ".UTF-8");
    #if defined(_WIN32)
    auto        args      = std::string();
    auto        argvector = std::vector<const char *>();
    const auto *argv      = wargv_convert(argc, wargv, args, argvector);
    #endif
    return u8main(argc, argv);
}
#endif

////////////////////////////////////////////////////////////////////////////////
// definition (source)
////////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
auto wargv_convert(int argc, const wchar_t *const *wargv, std::string &args,
                   std::vector<const char *> &argvector) -> const char *const *
{
    int len = 0;
    for (int i = 0; i < argc; ++i) {
        len += WideCharToMultiByte(CP_UTF8, NULL, wargv[i], -1, NULL, NULL, NULL, NULL);
    }
    args.resize(len);

    argvector.resize(argc);
    for (int i = 0, cur = 0; i < argc; ++i) {
        argvector[i] = &args[cur];
        cur += WideCharToMultiByte(CP_UTF8, NULL, wargv[i], -1, &args[cur], len - cur, NULL, NULL);
    }
    return argvector.data();
}
#endif