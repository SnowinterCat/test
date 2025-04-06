#pragma once
#include <test/3rdwrap/libsdl3/libsdl3_library.h>

#include <span>

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_LIBSDL3_BEGIN

auto TEST_3RDWRAP_API GetVkInstanceExtensions() -> std::span<const char *const>;

TEST_LIBSDL3_END
TEST_3RDWRAP_END
TEST_END