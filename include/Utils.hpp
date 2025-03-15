#pragma once

#include "Tile.hpp"
#include "Matrix.hpp"
#include "Grid.hpp"

namespace sas
{

    //Epsilon Tollerance
    bool areAlmostEqual(float a, float b, float epsilon = 1e-5f) noexcept;
    bool checkBoundaries(const std::pair<size_t, size_t>& p1, size_t p1Size, const std::pair<size_t, size_t>& p2, size_t p2Size) noexcept;

    size_t generateSeed() noexcept;

    float euclidianDistance2D(size_t x1, size_t y1, size_t x2, size_t y2) noexcept;

    std::pair<size_t, size_t> generateNextPos() noexcept;
    std::pair<size_t, size_t> generateNextPos(size_t x, size_t y, size_t range) noexcept;

    std::vector<std::pair<size_t, size_t>> chooseWeedCoords(const sas::Matrix<sas::Tile>& board);

    //If we add another parameter to the facory, I am chaning it
    //Enviroment
    // std::unique_ptr<sas::Enviroment> enviromentFactory(Grid &grid, size_t x, size_t y, sas::EnviromentType type, std::shared_ptr<DrawStrategy> strat = nullptr) noexcept;
    std::unique_ptr<sas::Enviroment> enviromentFactory(Grid &grid, const Position& n_pos, sas::EnviromentType type, std::shared_ptr<DrawStrategy> strat = nullptr) noexcept;

    //Plant
    std::unique_ptr<sas::Plant> plantFactory(Grid &grid, const std::pair<size_t, size_t>& n_pos, sas::PlantType type, std::shared_ptr<DrawStrategy> strat = nullptr) noexcept;  
    std::unique_ptr<sas::Plant> plantFactory(Grid &grid, size_t x, size_t y, sas::PlantType type, std::shared_ptr<DrawStrategy> strat = nullptr) noexcept;

} // namespace sas
