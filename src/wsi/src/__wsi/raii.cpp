#include <test/__wsi/raii.hpp>

TEST_BEGIN
TEST_WSI_BEGIN

void ContextDeleter::operator()(void *p)
{
    p ? SDL_Quit() : void();
}

void WindowDeleter::operator()(SDL_Window *p)
{
    p ? SDL_DestroyWindow(p) : void();
}

TEST_WSI_END
TEST_END