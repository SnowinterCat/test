#pragma once
#include <test/config.hpp>
#include <test/__stl/__config.hpp>
// Standard Library
#include <version>
// outptrlib
#if defined(__cpp_lib_out_ptr)
    #include <memory>
#else
    #include <memory>
    #include <ztd/out_ptr.hpp>
#endif

TEST_BEGIN
TEST_STL_BEGIN

namespace outptr
{
#if defined(__cpp_lib_out_ptr)
    using namespace ::std;
#else
    using namespace ::ztd::out_ptr;
#endif
} // namespace outptr

TEST_STL_END
TEST_END