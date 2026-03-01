#pragma once
#include <test/config.hpp>
#include <test/__stl/__config.hpp>
// Standard Library
#include <concepts>
#include <utility>

TEST_BEGIN
TEST_STL_BEGIN

template <std::invocable Func>
class [[nodiscard]] ScopeExit {
public:
    ScopeExit(Func func) : _func(std::move(func)) {}
    ~ScopeExit() { _func(); }

    ScopeExit(const ScopeExit &)            = delete;
    ScopeExit &operator=(const ScopeExit &) = delete;

    ScopeExit(ScopeExit &&rhs) noexcept : _func(std::exchange(rhs._func, {})) {}
    ScopeExit &operator=(ScopeExit &&rhs) noexcept
    {
        if (this != &rhs) {
            std::swap(_func, rhs._func);
        }
        return *this;
    }

protected:
    Func _func;
};

TEST_STL_END
TEST_END