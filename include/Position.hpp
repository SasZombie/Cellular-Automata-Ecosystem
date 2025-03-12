#pragma once
#include <cctype>


namespace sas
{
    struct Position
    {
        std::size_t x = 0, y = 0;
        std::size_t width = 0, height = 0;
    };
} // namespace sas
