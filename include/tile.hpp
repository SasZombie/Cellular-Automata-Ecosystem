#pragma once
#include <optional>
#include <random>
#include <memory>
#include "Plant.hpp"

namespace sas
{
    enum struct TileType
    {
        SNOW,
        DESERT,
        GRASS
    };

    struct Tile
    {
        TileType type;
        std::unique_ptr<Plant> plant;
    };
} // namespace sas
