#pragma once
#include <test/__core/__config.hpp>
// Standard Library
#include <string>
#include <vector>

TEST_BEGIN

/**
 * @brief
 *
 * @param str
 * @param argc
 * @param argvector
 * @return std::string
 */
TEST_CORE_API std::string string_to_argc_argv(const std::string &str, int &argc,
                                              std::vector<const char *> &argvector);

TEST_END
