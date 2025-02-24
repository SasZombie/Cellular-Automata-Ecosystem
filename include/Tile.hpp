#pragma once
#include <optional>
#include <random>
#include <memory>
#include <any>
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
        std::unique_ptr<Enviroment> env;
    };
} // namespace sas
