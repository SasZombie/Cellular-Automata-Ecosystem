#pragma once

#include "Tile.hpp"
#include "matrix.hpp"
#include "Grid.hpp"

namespace sas
{

    std::pair<size_t, size_t> generateSeed() noexcept;

    float euclidianDistance2D(size_t x1, size_t y1, size_t x2, size_t y2) noexcept;

    std::unique_ptr<sas::Plant> plantFactory(sas::Grid &grid, size_t x, size_t y, sas::PlatType type) noexcept;
    
    
    // sas::Enviroment enviromentFactory(Grid &grid, sas::PlatType type);

} // namespace sas
