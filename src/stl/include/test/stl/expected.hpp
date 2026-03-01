#pragma once
#include <test/config.hpp>
#include <test/__stl/__config.hpp>
// Standard Library
#include <version>
// expectedlib
#if defined(__cpp_lib_expected)
    #include <expected>
#else
    #include <zeus/expected.hpp>
#endif

TEST_BEGIN
TEST_STL_BEGIN

namespace expected
{
#if defined(__cpp_lib_expected)
    using namespace ::std;
#else
    using namespace ::zeus;
#endif
} // namespace expected

TEST_STL_END
TEST_END