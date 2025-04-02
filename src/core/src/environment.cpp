#include <test/config.h>
#include "test/core/environment.hpp"

#include <codecvt>
#include <locale>
#include <sstream>
#include <iterator>

TEST_BEGIN

std::string string_to_argc_argv(const std::string &str, int &argc,
                                std::vector<const char *> &argvector)
{
    std::istringstream       iss(str);
    std::vector<std::string> args = {std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>{}};

    argc = static_cast<int>(args.size());
    argvector.resize(argc);

    std::string buffer = {};
    for (auto &arg : args) {
        buffer.append(arg).append(1, '\0');
    }
    if (argc == 0) {
        return buffer;
    }

    argvector[0] = buffer.data();
    for (int i = 1; i < argc; ++i) {
        argvector[i] = argvector[i - 1] + args[i - 1].size() + 1;
    }
    return buffer;
}

TEST_END