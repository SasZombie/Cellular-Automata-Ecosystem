#include "../include/Utils.hpp"
#include "../include/Common.hpp"

#include <print>
#include <fstream>
#include <iostream>
#include <raylib.h>

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
    std::uniform_int_distribution<size_t> distX(std::max(x, range) - range, x + range);
    std::uniform_int_distribution<size_t> distY(std::max(y, range) - range, y + range);

    return std::make_pair(distX(genWithSeed), distY(genWithSeed));
}

bool sas::areAlmostEqual(float a, float b, float epsilon) noexcept
{
    return std::fabs(a - b) < epsilon;
}

bool sas::checkBoundaries(const std::pair<size_t, size_t>& p1, size_t p1Size, const std::pair<size_t, size_t>& p2, size_t p2Size) noexcept
{
    return euclidianDistance2D(p1.first, p1.second, p2.first, p2.second) > p1Size + p2Size;
}


//TODO: Maybe cosmin can make this with math such that we dont create 2 rectangles
bool sas::checkBoundaries(const Position& pos1, const Position& pos2) noexcept
{
    Rectangle rec1{static_cast<float>(pos1.x), static_cast<float>(pos1.y), static_cast<float>(pos1.width), static_cast<float>(pos1.height)};
    Rectangle rec2{static_cast<float>(pos2.x), static_cast<float>(pos2.y), static_cast<float>(pos2.width), static_cast<float>(pos2.height)};

    // std::print("Pos1: x = {} y = {} w = {} h = {}\n", pos1.x, pos1.y, pos1.width, pos1.height);
    // std::print("Pos2: x = {} y = {} w = {} h = {}\n", pos2.x, pos2.y, pos2.width, pos2.height);
    return CheckCollisionRecs(rec1, rec2);
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

std::unique_ptr<sas::Plant> sas::plantFactory(StaticGrid &grid, size_t x, size_t y, sas::PlantType type, std::shared_ptr<DrawStrategy> strat) noexcept
{
    // std::unique_ptr<sas::Plant> plant;

    // switch (type)
    // {
    // case PlantType::FLOWER:
    //     plant = std::make_unique<sas::Flower>(x, y, strat);
    //     break;
    // case PlantType::WEED:
    //     plant = std::make_unique<sas::Weed>(x, y, strat);
    //     break;
    // case PlantType::TREE:
    //     plant = std::make_unique<sas::Tree>(x, y, strat);
    //     break;
    // default:
    //     std::cerr << "Warning! Unimplemented PlantType! Returning nullptr.\n";
    //     return nullptr;
    // }

    
    // // addToGrid(grid, plant.get());
    // return plant;
}


std::unique_ptr<sas::Plant> sas::plantFactory(sas::StaticGrid &grid, const std::pair<size_t, size_t>& n_pos, sas::PlantType type, std::shared_ptr<DrawStrategy> strat) noexcept
{
    return plantFactory(grid, n_pos.first, n_pos.second, type, std::move(strat));
}

std::unique_ptr<sas::Enviroment> sas::enviromentFactory(StaticGrid &grid, const Position& n_pos, sas::EnviromentType type, std::shared_ptr<DrawStrategy> strat) noexcept
{
    // std::unique_ptr<sas::Enviroment> env;

    // switch (type)
    // {
    // case EnviromentType::WATER:    
    //     env = std::make_unique<sas::Water>();
    //     break;
    // default:
    //     std::cerr << "Unimplemented! Returning nullptr instead!\n";
    //     break;
    // }

    // if(strat)
    // {
    //     env->setDrawStrategy(std::move(strat));
    // }
    // else
    // {
    //     env->setDrawStrategy(std::make_unique<PlaceholderDrawStrategy>());
    // }

    // env->pos = n_pos;
    // // addToGrid(grid, env.get());

    // return env;
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
