#pragma once
#include <cctype>

using size_t = std::size_t;

namespace sas
{
    struct Position
    {
        size_t x = 0, y = 0;
        size_t width = 0, height = 0;
    };
} // namespace sas
