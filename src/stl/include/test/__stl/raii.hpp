#pragma once
#include <test/config.hpp>
#include <test/__stl/__config.hpp>
// Standard Library
#include <cstdio>
#include <memory>

TEST_BEGIN
TEST_STL_BEGIN

struct TEST_STL_API FileCloser {
    void operator()(FILE *file);
};

namespace raii
{
    using File = std::unique_ptr<FILE, FileCloser>;
}

TEST_STL_END
TEST_END