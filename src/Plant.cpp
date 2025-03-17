#include "../include/Plant.hpp"
#include <random>
#include <print>
#include "Utils.hpp"
#include <iostream>
#include "../include/Utils.hpp"
#include "raylib.h"
#include "Common.hpp"
#include "Plant.hpp"

static constexpr std::size_t FlowerDaysAlive = 5;
static constexpr std::size_t TreeDaysAlive   = 10;
static constexpr std::size_t WeedDaysAlive   = 3;
 
static constexpr std::size_t FlowerRangeSeeds = 50;
static constexpr std::size_t FlowerWaterRange = 25;
static constexpr std::size_t FlowerSize = 5;
static constexpr std::size_t FlowerNumberOfSeeds = 3;
static constexpr std::size_t FlowerWaterConsumption = 10;
 
static constexpr std::size_t TreeRangeSeeds = 100;
static constexpr std::size_t TreeWaterRange = 40;
static constexpr std::size_t TreeSize = 50;
static constexpr std::size_t TreeNumberOfSeeds = 1;
static constexpr std::size_t TreeWaterConsumption = 25;

static constexpr std::size_t WeedRangeSeeds = 60;
static constexpr std::size_t WeedWaterRange = 15;
static constexpr std::size_t WeedSize = 5;
static constexpr std::size_t WeedNumberOfSeeds = 15;
static constexpr std::size_t WeedWaterConsumption = 12;

sas::Plant::Plant(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat, size_t n_nrOfSeeds, size_t n_rangeSpreadingSeeds, size_t n_rangeWater, size_t n_size, size_t n_waterConsumption) noexcept
    : Entity(x, y),
      nrOfSeeds(n_nrOfSeeds),
      rangeSpreadingSeeds(n_rangeSpreadingSeeds),
      rangeWater(n_rangeWater),
      size(n_size),
      waterConsumption(n_waterConsumption)
{
    this->setDrawStrategy(strat ? strat : std::make_shared<PlaceholderDrawStrategy>());
}

std::vector<sas::Position> sas::Plant::reproduce() const noexcept
{

    std::vector<Position> vecs;

    for (size_t i = 0; i < this->nrOfSeeds; ++i)
    {
        const auto [x, y] = generateNextPos(pos.x, pos.y, this->rangeSpreadingSeeds);
        vecs.push_back({x, y, this->pos.width, this->pos.height});
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

sas::PlantType sas::Weed::getPlantType() const noexcept
{
    return PlantType::WEED;
}

sas::Flower::Flower(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(x, y, strat, FlowerNumberOfSeeds, FlowerRangeSeeds, FlowerWaterRange, FlowerSize, FlowerWaterConsumption)
{
}

sas::Tree::Tree(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(x, y, strat, TreeNumberOfSeeds, TreeRangeSeeds, TreeWaterRange, TreeSize, TreeWaterConsumption)
{
}

sas::Weed::Weed(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(x, y, strat, WeedNumberOfSeeds, WeedRangeSeeds, WeedWaterRange, WeedSize, WeedWaterConsumption)
{
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

size_t sas::Flower::getWaterConsumption() const noexcept
{
    return waterConsumption;
}

size_t sas::Tree::getWaterConsumption() const noexcept
{
    return waterConsumption;
}

size_t sas::Weed::getWaterConsumption() const noexcept
{
    return waterConsumption;
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