#include "../include/Plant.hpp"
#include <random>
#include <print>
#include "../include/ConfigManager.hpp"
#include "Utils.hpp"
#include <iostream>
#include "../include/Utils.hpp"
#include "raylib.h"
#include "Common.hpp"
#include "Plant.hpp"

sas::Plant::Plant(const Position &p, std::shared_ptr<DrawStrategy> strat, size_t n_nrOfSeeds, size_t n_rangeSpreadingSeeds, size_t n_rangeWater, size_t n_size, size_t n_waterConsumption) noexcept
    : Entity(p),
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
    vecs.reserve(this->nrOfSeeds);
    size_t range = this->rangeSpreadingSeeds * cellSize;

    std::cout << "Range is: " << range << '\n';

    for (size_t i = 0; i < this->nrOfSeeds; ++i)
    {
        const auto [x, y] = generateNextPos(pos.x, pos.y, range);
        vecs.emplace_back(x, y, this->pos.width, this->pos.height);
    }

    return vecs;
}

std::unique_ptr<sas::Plant> sas::Tree::createOffspring(const Position &p) const noexcept
{
    return std::make_unique<Tree>(p, getDrawStartegy());
}

std::unique_ptr<sas::Plant> sas::Flower::createOffspring(const Position &p) const noexcept
{
    return std::make_unique<Flower>(p, getDrawStartegy());
}

std::unique_ptr<sas::Plant> sas::Weed::createOffspring(const Position &p) const noexcept
{
    return std::make_unique<Weed>(p, getDrawStartegy());
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

sas::Flower::Flower(const Position &p, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(p, strat, ConfigManager::get().Flower.NumberOfSeeds, ConfigManager::get().Flower.RangeSeeds, ConfigManager::get().Flower.WaterRange,
            ConfigManager::get().Flower.Size, ConfigManager::get().Flower.WaterConsumption)
{
}

sas::Tree::Tree(const Position &p, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(p, strat, ConfigManager::get().Tree.NumberOfSeeds, ConfigManager::get().Tree.RangeSeeds, ConfigManager::get().Tree.WaterRange,
            ConfigManager::get().Tree.Size, ConfigManager::get().Tree.WaterConsumption)
{
}

sas::Weed::Weed(const Position &p, std::shared_ptr<DrawStrategy> strat) noexcept
    : Plant(p, strat, ConfigManager::get().Weed.NumberOfSeeds, ConfigManager::get().Weed.RangeSeeds, ConfigManager::get().Weed.WaterRange,
            ConfigManager::get().Weed.Size, ConfigManager::get().Weed.WaterConsumption)
{
}

size_t sas::Flower::range() const noexcept
{
    return this->rangeWater;
}

size_t sas::Weed::range() const noexcept
{
    return this->rangeWater;
}

size_t sas::Tree::range() const noexcept
{
    return this->rangeWater;
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
    return daysAlive >= ConfigManager::get().Flower.DaysAlive;
}

bool sas::Tree::willWither() const noexcept
{
    return daysAlive >= ConfigManager::get().Tree.DaysAlive;
}

bool sas::Weed::willWither() const noexcept
{
    return daysAlive >= ConfigManager::get().Weed.DaysAlive;
}
