#pragma once
#include <optional>
#include <random>
#include <memory>
#include <variant>

#include "Enviroment.hpp"
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
        std::variant<std::unique_ptr<Plant>, std::unique_ptr<Enviroment>> occupant;
    };
} // namespace sas
