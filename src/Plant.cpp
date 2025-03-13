#include "../include/Plant.hpp"
#include <random>
#include <print>
#include "Utils.hpp"
#include <iostream>
#include "../include/Utils.hpp"
#include "raylib.h"
#include "Common.hpp"
#include "Plant.hpp"

sas::Plant::Plant(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat, size_t n_nrOfSeeds, size_t n_rangeSpreadingSeeds, size_t n_rangeWater, size_t n_size) noexcept
    : Entity(x, y),
      nrOfSeeds(n_nrOfSeeds),
      rangeSpreadingSeeds(n_rangeSpreadingSeeds),
      rangeWater(n_rangeWater),
      size(n_size)
{
    this->setDrawStrategy(strat ? strat : std::make_shared<PlaceholderDrawStrategy>());
}

std::vector<std::pair<size_t, size_t>> sas::Plant::reproduce() const noexcept
{

    std::vector<std::pair<size_t, size_t>> vecs;

    for (size_t i = 0; i < this->nrOfSeeds; ++i)
    {
        vecs.push_back(generateNextPos(pos.x, pos.y, this->rangeSpreadingSeeds));
    }

    return vecs;
}

sas::PlantType sas::Flower::getPlantType() const noexcept
{
    return PlantType::FLOWER;
}

sas::PlantType sas::Tree::getPlantType() const noexcept
{
    return PlantType::TREE;
}

sas::Flower::Flower(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(x, y, strat, FlowerNumberOfSeeds, FlowerRangeSeeds, FlowerWaterRange, FlowerSize)
{
}

sas::Tree::Tree(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(x, y, strat, TreeNumberOfSeeds, TreeRangeSeeds, TreeWaterRange, TreeSize)
{
}

sas::Weed::Weed(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(x, y, strat, WeedNumberOfSeeds, WeedRangeSeeds, WeedWaterRange, WeedSize)
{
}
sas::PlantType sas::Weed::getPlantType() const noexcept
{
    return PlantType::WEED;
}

size_t sas::Flower::range() const noexcept
{
    return 10;
}

size_t sas::Weed::range() const noexcept
{
    return 10;
}

size_t sas::Tree::range() const noexcept
{
    return 10;
}
// TODO: return value
size_t sas::Flower::waterConsumption() const noexcept
{
    return 1;
}
// TODO: return value
size_t sas::Tree::waterConsumption() const noexcept
{
    return 1;
}
// TODO: return value
size_t sas::Weed::waterConsumption() const noexcept
{
    return 1;
}

bool sas::Flower::willWither() const noexcept
{
    return daysAlive >= FlowerDaysAlive;
}

bool sas::Tree::willWither() const noexcept
{
    return daysAlive >= TreeDaysAlive;
}

bool sas::Weed::willWither() const noexcept
{
    return daysAlive >= WeedDaysAlive;
}