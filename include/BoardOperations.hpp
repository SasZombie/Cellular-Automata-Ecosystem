#pragma once
#include "Matrix.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include "Plant.hpp"

namespace sas
{

    void SetUpBoard(Matrix<Tile> &board);
    void SetUpBoardPerlin(Matrix<Tile> &board, size_t seed);

    void killPlants(StaticGrid &grid, std::vector<std::unique_ptr<Plant>> &plants) noexcept;
    void multiplyPlants(sas::DynamicGrid &plantGrid, sas::StaticGrid &enviromentGrid, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept;
    void spawnWeed(StaticGrid &grid, const Matrix<Tile> &board, std::vector<std::unique_ptr<Plant>> &plants) noexcept;
    bool hasSpaceAgainstPlants(const Position &p, const std::vector<std::unique_ptr<Plant>> &plants, const DynamicGrid& plantGrid, const StaticGrid& envGrid) noexcept;
    bool isNearWater(const Position &pos, const StaticGrid &waterCells, size_t maxRange) noexcept;

    void SetUpWater(std::vector<std::unique_ptr<Enviroment>> &waters, StaticGrid &grid) noexcept;
    void SetUpWaterNoise(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::StaticGrid &grid, size_t seed) noexcept;

    void addPlant(std::unique_ptr<sas::Plant> plant, sas::DynamicGrid &plantGrid, std::vector<std::unique_ptr<sas::Plant>>& plants, int spatialCellSize = 50);
    void addWater(int gridX, int gridY, std::vector<std::unique_ptr<sas::Enviroment>> &water, sas::StaticGrid &waterCells) noexcept;

}