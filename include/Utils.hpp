#pragma once

#include "Tile.hpp"
#include "Matrix.hpp"
#include "Grid.hpp"

namespace sas
{

    //Epsilon Tollerance
    bool areAlmostEqual(float a, float b, float epsilon = 1e-5f) noexcept;
    bool checkBoundaries(const Position& pos1, const Position& pos2) noexcept;
    bool checkBoundaries(const std::pair<size_t, size_t>& p1, size_t p1Size, const std::pair<size_t, size_t>& p2, size_t p2Size) noexcept;

    std::vector<Position> chooseWeedCoords();

    size_t generateSeed() noexcept;

    float euclidianDistance2D(size_t x1, size_t y1, size_t x2, size_t y2) noexcept;

    std::pair<size_t, size_t> generateNextPos() noexcept;
    std::pair<size_t, size_t> generateNextPos(size_t x, size_t y, size_t range) noexcept;

} // namespace sas
