#include "../include/Utils.hpp"
#include "../include/Common.hpp"
#include "../include/ConfigManager.hpp"


#include <print>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <random>

//It would be cool
//If we use this: https://www.youtube.com/watch?v=xMdwK9p5qOw

static std::random_device rd;
static std::mt19937 genForSeed(rd());
static std::uniform_int_distribution<size_t> seedDist(0, 501);

static std::mt19937 genWithSeed;
static std::uniform_int_distribution<size_t> distWidth(0, ScreenWidth);
static std::uniform_int_distribution<size_t> distHeight(0, ScreenHeight);
static std::uniform_int_distribution<size_t> distCellOffset(0, cellSize - 1);

static std::uniform_int_distribution<size_t> plantXYDistribution;


//0.001sf = chance to spawn a random plant
static constexpr float weedChance = 0.001f;
std::bernoulli_distribution weedDist(weedChance);

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

std::pair<size_t, size_t> sas::generateNextPos(size_t minX, size_t maxX, size_t minY, size_t maxY) noexcept
{
    
    plantXYDistribution.param(std::uniform_int_distribution<size_t>::param_type(minX, maxX));

    size_t x = plantXYDistribution(genWithSeed);

    plantXYDistribution.param(std::uniform_int_distribution<size_t>::param_type(minY, maxY));
    size_t y = plantXYDistribution(genWithSeed);

    return std::make_pair(x, y);
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
//Depricated
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


std::vector<sas::Position> sas::chooseWeedCoords()
{
    static const size_t sizeWeed = ConfigManager::get().Weed.Size;

    std::vector<Position> elements;

    for (size_t y = 0; y < HeightCells; ++y)
    {
        for (size_t x = 0; x < WidthCells; ++x)
        {
            if (weedDist(genWithSeed))
            {
                size_t offsetX = distCellOffset(genWithSeed);
                size_t offsetY = distCellOffset(genWithSeed);
                size_t posX = x * cellSize + offsetX;
                size_t posY = y * cellSize + offsetY;

                posX = std::min(posX, ScreenWidth - 1);
                posY = std::min(posY, ScreenHeight - 1);


                elements.emplace_back(Position{posX, posY, sizeWeed, sizeWeed});
            }
        }
    }

    return elements;
}
