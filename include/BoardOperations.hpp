#pragma once
#include "Matrix.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include "Plant.hpp"

namespace sas
{

    void SetUpBoard(Matrix<Tile> &board);
    void SetUpBoardPerlin(Matrix<Tile> &board);

    void killPlants(Grid &grid, std::vector<std::unique_ptr<Plant>> &plants) noexcept;
    void multiply(Grid &grid, std::vector<std::unique_ptr<Plant>> &plants) noexcept;
    void spawnWeed(Grid &grid, const Matrix<Tile> &board, std::vector<std::unique_ptr<Plant>> &plants) noexcept;
    void SetUpWater(std::vector<std::unique_ptr<Enviroment>> &waters, Grid &grid) noexcept;
    void SetUpWaterNoise(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::Grid &grid, size_t seed) noexcept;


}