#pragma once
#include "Matrix.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include "Plant.hpp"

namespace sas
{

    void setUpBoard(Matrix<Tile> &board) noexcept;
    void setUpBoardPerlin(Matrix<Tile> &board, size_t seed) noexcept;

    void setUpWater(std::vector<std::unique_ptr<Enviroment>> &waters, StaticGrid &grid) noexcept;
    void setUpWaterNoise(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::StaticGrid &grid, size_t seed) noexcept;

    //Maybe we can do better than spawning some plants randomlly 
    void setUpInitialPlants(std::vector<std::unique_ptr<Plant>> &plants, DynamicGrid &grid, const StaticGrid &waterCells, std::vector<std::unique_ptr<sas::Enviroment>> &water) noexcept;
    

    //these functions have too many damn params
    void killPlants(sas::DynamicGrid &plantGrid, std::vector<std::unique_ptr<sas::Plant>> &plants, std::vector<std::unique_ptr<sas::Enviroment>> &water) noexcept;
    void multiplyPlants(sas::DynamicGrid &plantGrid, sas::StaticGrid &enviromentGrid, std::vector<std::unique_ptr<sas::Enviroment>> &water, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept;
    void spawnWeed(sas::DynamicGrid &plantGrid, sas::StaticGrid &enviromentGrid, std::vector<std::unique_ptr<sas::Enviroment>> &water, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept;
    bool hasSpaceAgainstPlants(const Position &p, const std::vector<std::unique_ptr<Plant>> &plants, const DynamicGrid& plantGrid, const StaticGrid& envGrid) noexcept;
    
    
    void addPlant(std::unique_ptr<sas::Plant> plant, sas::DynamicGrid &plantGrid, std::vector<std::unique_ptr<sas::Plant>>& plants, int spatialCellSize = 50);
    void addWater(int gridX, int gridY, std::vector<std::unique_ptr<sas::Enviroment>> &water, sas::StaticGrid &waterCells) noexcept;
    
    std::optional<size_t> isNearWater(const Position &pos, const StaticGrid &waterCells, std::vector<std::unique_ptr<sas::Enviroment>> &water, const Plant* p) noexcept;
    std::optional<size_t> getClosestWaterCell(const Position &pos, const StaticGrid &waterCells, std::vector<std::unique_ptr<sas::Enviroment>> &water, int maxRange) noexcept;
}