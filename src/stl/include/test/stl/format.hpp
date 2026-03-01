#pragma once
#include <test/config.hpp>
#include <test/__stl/__config.hpp>
// Standard Library
#include <version>
// fmtlib
#if __cpp_lib_format >= 202311L
    #include <format>
#else
    #include <fmt/xchar.h>
#endif

TEST_BEGIN
TEST_STL_BEGIN

namespace fmt
{
#if __cpp_lib_format >= 202311L
    using namespace ::std;
#else
    using namespace ::fmt;
#endif
} // namespace fmt

TEST_STL_END
TEST_END