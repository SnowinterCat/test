#include "luancher/main.hpp"

#if defined(TEST_USE_LUANCHER) && TEST_USE_LUANCHER && !TEST_LUANCHER_EXPORTS
#else
    #if defined(_WIN32)
        #include <Windows.h>

        #include <clocale>
        #include <string>
        #include <vector>
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

int wmain(int argc, const wchar_t *const *wargv)
{
    std::setlocale(LC_ALL, ".UTF-8");
    auto        args      = std::string();
    auto        argvector = std::vector<const char *>();
    const auto *argv      = wargv_convert(argc, wargv, args, argvector);
    return u8main(argc, argv);
}
    #else
        #include <clocale>
int main(int argc, const char *const *argv)
{
    std::setlocale(LC_ALL, ".UTF-8");
    return u8main(argc, argv);
}
    #endif
#endif