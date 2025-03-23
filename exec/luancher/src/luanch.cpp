#include <test/config.h>
#include "luanch.hpp"
// 系统库
#if defined(_WIN32)
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif
// 标准库
#include <memory>
#include <filesystem>
#include <format>
#include <iostream>
// 三方库

// 项目内依赖

// 当前目录

////////////////////////////////////////////////////////////////////////////////
// declaration
////////////////////////////////////////////////////////////////////////////////

struct LibraryDeleter {
#if defined(_WIN32)
    void operator()(HMODULE library)
    {
        if (library) {
            FreeLibrary(library);
        }
    }
#else
    void operator()(void *library)
    {
        if (library) {
            dlclose(library);
        }
    }
#endif
};

/**
 * @brief 根据基础名查找动态库路径
 *
 * @param baseName
 * @param dir
 * @param errc
 * @return std::filesystem::path
 */
auto find_dynamic_library_path(const char *baseName, std::filesystem::path dir,
                               std::error_code &errc) -> std::filesystem::path;

////////////////////////////////////////////////////////////////////////////////
// definition (header)
////////////////////////////////////////////////////////////////////////////////

int luanch_main(const char *baseName, int argc, const char *const *argv)
{
    typedef int (*U8MAIN)(int argc, const char *const *argv);
    U8MAIN u8Main = nullptr;
    auto   errc   = std::error_code();

    auto execDir = std::filesystem::weakly_canonical(argv[0]).parent_path();
    auto dllPath = find_dynamic_library_path(baseName, execDir, errc);
    if (errc) {
        std::cout << std::format("find_dynamic_library_path error, code: {}, info", errc.value(),
                                 errc.message())
                  << std::endl;
        return errc.value();
    }

    auto oldWorkPath = std::filesystem::current_path();
    auto newWorkPath = execDir.parent_path().parent_path() / "lib" /
                       std::filesystem::relative(execDir, execDir.parent_path());
    if (std::filesystem::current_path(newWorkPath, errc); errc) {
        std::cout << std::format("std::filesystem::current_path error, code: {}, info: {}",
                                 errc.value(), errc.message())
                  << std::endl;
        return errc.value();
    }

#if defined(_WIN32)
    auto lib = std::unique_ptr<std::remove_pointer_t<HMODULE>, LibraryDeleter>(
        LoadLibraryW((dllDir / baseName).c_str()));
    if (lib == nullptr) {
        std::printf("%s", std::format("LoadLibraryW error, code: {}\n", GetLastError()).c_str());
        return GetLastError();
    }
    u8Main = reinterpret_cast<U8MAIN>(GetProcAddress(lib.get(), "u8main")); // NOLINT
    if (u8Main == nullptr) {
        std::printf("%s", std::format("GetProcAddress error, code: {}\n", GetLastError()).c_str());
        return GetLastError();
    }
#else
    auto lib = std::unique_ptr<void, LibraryDeleter>(dlopen(dllPath.c_str(), RTLD_LAZY));
    if (lib == nullptr) {
        std::cout << std::format("dlopen error, info: {}", dlerror()) << std::endl;
        return -1;
    }
    u8Main = reinterpret_cast<U8MAIN>(dlsym(lib.get(), "u8main")); // NOLINT
    if (u8Main == nullptr) {
        std::cout << std::format("dlsym error, info: {}", dlerror()) << std::endl;
        return -1;
    }
#endif

    std::filesystem::current_path(execDir);
    int result = u8Main(argc, argv);
    std::filesystem::current_path(oldWorkPath);
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// definition (source)
////////////////////////////////////////////////////////////////////////////////

auto find_dynamic_library_path(const char *baseName, std::filesystem::path dir,
                               std::error_code &errc) -> std::filesystem::path
{
#if defined(_WIN32)
    auto fileName = std::string(baseName) + ".dll";
#elif defined(__linux__)
    auto fileName = std::string("lib") + baseName + ".so";
#elif defined(__APPLE__)
    auto fileName = std::string("lib") + baseName + ".dylib";
#else
    #error "not support system"
#endif
    if (std::filesystem::is_regular_file(dir / fileName, errc)) {
        return dir / fileName;
    }
    return {};
}
