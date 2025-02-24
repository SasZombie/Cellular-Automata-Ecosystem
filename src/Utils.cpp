#include "../include/Utils.hpp"
#include <iostream>

static std::random_device rd;
static std::mt19937 genForSeed(rd());
static std::uniform_int_distribution<size_t> seedDist(0, 501);

static std::mt19937 genWithSeed;
static std::uniform_int_distribution<size_t> dist(0, 29);
static std::uniform_int_distribution<size_t> dist2(0, 19);

size_t sas::generateSeed() noexcept
{
    size_t seed = seedDist(genForSeed);

    genWithSeed.seed(seed);
    return seed;
}

std::pair<size_t, size_t> sas::generateNextPos() noexcept
{
    return std::make_pair(dist(genWithSeed), dist2(genWithSeed));
}


std::pair<size_t, size_t> sas::generateNextPos(size_t x, size_t y, size_t range, size_t minDidst) noexcept
{
    const auto validCoord = [&](size_t center) -> size_t{
        size_t total = 2 * range + 1 - (2 * minDidst + 1);
        std::uniform_int_distribution<size_t> dist(0, total);

        size_t randomValue = dist(genWithSeed);

        if(randomValue >= range - minDidst)
        {
            randomValue = randomValue + 2 * minDidst + 1;
        }

        return center - range + randomValue;
    };

    return {validCoord(x), validCoord(y)};
}

bool sas::checkBoundaries(const std::pair<size_t, size_t>& p1, const std::pair<size_t, size_t>& p2) noexcept
{
    return euclidianDistance2D(p1.first, p1.second, p2.first, p2.second);
}

float sas::euclidianDistance2D(size_t x1, size_t y1, size_t x2, size_t y2) noexcept
{
    return std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2));
}

std::unique_ptr<sas::Plant> sas::plantFactory(sas::Grid &grid, size_t x, size_t y, sas::PlatType type) noexcept
{
    std::unique_ptr<sas::Plant> plant;

    switch (type)
    {
    case sas::PlatType::FLOWER:    
        plant = std::make_unique<sas::Flower>();
        break;
    case sas::PlatType::WEED:
        plant = std::make_unique<sas::Weed>();
        break;
    default:
        std::cerr << "Unimplemented! Returning a flower instead!\n";
        break;
    }

    plant->pos = {x, y};
    sas::addToGrid(grid, plant.get());

    return plant;
}