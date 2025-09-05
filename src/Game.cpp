#include "../include/Game.hpp"
#include "../include/BoardOperations.hpp"
#include "../include/Common.hpp"

sas::Game::Game(size_t n_seed) 
    : seed(n_seed), board(WidthCells, HeightCells)
{
    setUp(seed);
}

void sas::Game::setUp(size_t n_seed)
{ 
    clearAll();
   
    this->seed = n_seed;
    setUpBoardPerlin(board, seed);
    setUpWaterNoise(enviroment, enviromentGrid, seed);
    setUpInitialPlants(plants, plantGrid, enviromentGrid, enviroment);
}

void sas::Game::passTime() noexcept
{
    multiplyPlants(plantGrid, enviromentGrid, enviroment, plants);
    spawnWeed(plantGrid, enviromentGrid, enviroment, plants);

    for (auto &plt : plants)
    {
        plt->daysAlive++;
    }

    killPlants(plantGrid, plants, enviroment);
}

const std::vector<std::unique_ptr<sas::Plant>> &sas::Game::plantsRef() const noexcept
{
    return plants;
}

const std::vector<std::unique_ptr<sas::Enviroment>> &sas::Game::enviromentRef() const noexcept
{
    return enviroment;
}

const sas::Matrix<sas::Tile> &sas::Game::boardRef() const noexcept
{
    return board;
}

void sas::Game::clearAll() noexcept
{
    board = Matrix<Tile>(WidthCells, HeightCells);
    enviroment.clear();
    enviromentGrid.clear();
    plants.clear();
    plantGrid.clear();
}