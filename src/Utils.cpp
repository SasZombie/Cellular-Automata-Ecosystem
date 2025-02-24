#include "../include/Utils.hpp"
#include <iostream>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<size_t> dist(0, 29);
static std::uniform_int_distribution<size_t> dist2(0, 19);

std::pair<size_t, size_t> sas::generateSeed() noexcept
{
    return std::make_pair(dist(gen), dist2(gen));
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