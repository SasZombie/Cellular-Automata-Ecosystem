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

    class Tile : public Entity
    {
    public:
        TileType type;
    };
} // namespace sas
