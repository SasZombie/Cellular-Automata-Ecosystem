#include "../include/Utils.hpp"
#include "../include/Common.hpp"
#include <print>
#include <iostream>

static std::random_device rd;
static std::mt19937 genForSeed(rd());
static std::uniform_int_distribution<size_t> seedDist(0, 501);

static std::mt19937 genWithSeed;
static std::uniform_int_distribution<size_t> distWidth(0, ScreenWidth);
static std::uniform_int_distribution<size_t> distHeight(0, ScreenHeight);

//0.06f = chance to spawn a random plant
static constexpr float weedChance = 0.06f;
static std::binomial_distribution<size_t> distTiles(WidthHeightCells, weedChance); 

size_t sas::generateSeed() noexcept
{
    size_t seed = seedDist(genForSeed);

    genWithSeed.seed(seed);
    return seed;
}

std::pair<size_t, size_t> sas::generateNextPos() noexcept
{
    return std::make_pair(distWidth(genWithSeed), distHeight(genWithSeed));
}

std::pair<size_t, size_t> sas::generateNextPos(size_t x, size_t y, size_t range) noexcept
{
    std::uniform_int_distribution<size_t> distX(x, x + range);
    std::uniform_int_distribution<size_t> distY(y, y + range);

    return {distX(genWithSeed), distY(genWithSeed)};
}

bool sas::areAlmostEqual(float a, float b, float epsilon) noexcept
{
    return std::fabs(a - b) < epsilon;
}

bool sas::checkBoundaries(const std::pair<size_t, size_t>& p1, size_t p1Size, const std::pair<size_t, size_t>& p2, size_t p2Size) noexcept
{
    return euclidianDistance2D(p1.first, p1.second, p2.first, p2.second) > p1Size + p2Size;
}

float sas::euclidianDistance2D(size_t x1, size_t y1, size_t x2, size_t y2) noexcept
{
    size_t distX;
    if(x2 > x1)
        distX = x2 - x1;
    else
        distX = x1 - x2;

    size_t distY;
    if(y2 > y1)
        distY = y2 - y1;
    else
        distY = y1 - y2;
    return std::sqrt(std::pow(distX, 2) + std::pow(distY, 2));
}

std::unique_ptr<sas::Plant> sas::plantFactory(sas::Grid &grid, size_t x, size_t y, sas::PlantType type, std::shared_ptr<DrawStrategy> strat) noexcept
{
    std::unique_ptr<sas::Plant> plant;

    switch (type)
    {
    case PlantType::FLOWER:    
        plant = std::make_unique<sas::Flower>();
        break;
    case PlantType::WEED:
        plant = std::make_unique<sas::Weed>();
        break;
    case PlantType::TREE:
        plant = std::make_unique<sas::Tree>();
        break;
    default:
        plant = std::make_unique<sas::Flower>();
        std::cerr << "Unimplemented! Returning a flower instead!\n";
        break;
    }

    if(strat)
    {
        plant->setDrawStrategy(std::move(strat));
    }
    else
    {
        plant->setDrawStrategy(std::make_unique<PlaceholderDrawStrategy>());
    }
    plant->pos = {x, y};
    addToGrid(grid, plant.get());

    return plant;
}

std::unique_ptr<sas::Plant> sas::plantFactory(sas::Grid &grid, const std::pair<size_t, size_t>& n_pos, sas::PlantType type, std::shared_ptr<DrawStrategy> strat) noexcept
{
    return plantFactory(grid, n_pos.first, n_pos.second, type, std::move(strat));
}

std::unique_ptr<sas::Enviroment> sas::enviromentFactory(Grid &grid, size_t x, size_t y, sas::EnviromentType type, std::shared_ptr<DrawStrategy> strat) noexcept
{
    std::unique_ptr<sas::Enviroment> env;

    switch (type)
    {
    case EnviromentType::WATER:    
        env = std::make_unique<sas::Water>();
        break;
    default:
        std::cerr << "Unimplemented! Returning a flower instead!\n";
        break;
    }

    if(strat)
    {
        env->setDrawStrategy(std::move(strat));
    }
    else
    {
        env->setDrawStrategy(std::make_unique<PlaceholderDrawStrategy>());
    }

    env->pos = {x, y};
    sas::addToGrid(grid, env.get());

    return env;
}


std::unique_ptr<sas::Enviroment> sas::enviromentFactory(Grid &grid, const std::pair<size_t, size_t>& n_pos, sas::EnviromentType type, std::shared_ptr<DrawStrategy> strat) noexcept
{
    return enviromentFactory(grid, n_pos.first, n_pos.second, type, std::move(strat));
}

std::vector<std::pair<size_t, size_t>> sas::chooseWeedCoords(const sas::Matrix<sas::Tile>& board)
{

    std::vector<std::pair<size_t, size_t>> elements;
    elements.reserve(static_cast<size_t>(WidthHeightCells * weedChance));

    size_t numSelected = distTiles(genWithSeed);
    std::uniform_int_distribution<size_t> selectedDist(0, WidthHeightCells - 1);
    
    for(size_t i = 0; i < numSelected; ++i)
    {
        size_t index = selectedDist(genWithSeed);
        elements.push_back(board(index).getPosition());
    }
    
    return elements;
}