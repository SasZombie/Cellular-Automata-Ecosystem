#pragma once
#include <cctype>
#include <tuple>

namespace sas
{
    struct Position
    {
        std::size_t x = 0, y = 0;
        std::size_t width = 0, height = 0;

        auto operator<=>(const Position& other) const
        {
            return std::tie(x, y, width, height) <=> std::tie(other.x, other.y, other.width, other.height);
        }

        bool operator==(const Position& other) const = default;

    };
} // namespace sas
