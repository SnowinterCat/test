#include <test/__stl/raii.hpp>

TEST_BEGIN
TEST_STL_BEGIN

void FileCloser::operator()(FILE *file)
{
    if (file != nullptr) {
        std::fclose(file);
    }
}

TEST_STL_END
TEST_END