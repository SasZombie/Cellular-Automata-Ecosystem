#pragma once
#include <cctype>

using size_t = std::size_t;

namespace sas
{
    struct Position
    {
        size_t x, y;
        size_t width, height;
    };
} // namespace sas
