#include <random>
#include <print>
#include <iostream>
#include "Common.hpp"
#include "Utils.hpp"
#include "Plant.hpp"
#include "ConfigManager.hpp"

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

// Δx ∼ N(μ, σ^2)
// μ = 0 ---> mutation average out
// σ = standard dev ---> how big/lagrge/impactful is mutation
static size_t mutateGaussian(size_t original, double stdev, int minVal = 0, int maxVal = INT_MAX)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::normal_distribution d(0.0, stdev);

    //x' = x + round(Δx)
    int delta = static_cast<int>(std::round(d(gen)));
    int mutated = static_cast<int>(original) + delta;

    mutated = std::clamp(mutated, minVal, maxVal);

    return static_cast<size_t>(mutated);
}

std::vector<sas::Position> sas::Plant::reproduce() const noexcept
{
    std::vector<Position> vecs;
    vecs.reserve(this->nrOfSeeds);
    size_t range = this->rangeSpreadingSeeds * cellSize;

    size_t minX = (pos.x >= range) ? pos.x - range : 0;
    size_t maxX = std::min(pos.x + range, ScreenWidth - 1);
    size_t minY = (pos.y >= range) ? pos.y - range : 0;
    size_t maxY = std::min(pos.y + range, ScreenHeight - 1);

    for (size_t i = 0; i < this->nrOfSeeds; ++i)
    {
        const auto [x, y] = generateNextPos(minX, maxX, minY, maxY);
        vecs.emplace_back(x, y, this->pos.width, this->pos.height);
    }

    return vecs;
}

std::unique_ptr<sas::Plant> sas::Tree::createOffspring(const Position &p) const noexcept
{
    std::unique_ptr<Tree> tree = std::make_unique<Tree>(p, getDrawStartegy());

    tree->nrOfSeeds = mutateGaussian(this->nrOfSeeds, 1.5);

    return tree;
}

std::unique_ptr<sas::Plant> sas::Flower::createOffspring(const Position &p) const noexcept
{
    std::unique_ptr<Flower> flower = std::make_unique<Flower>(p, getDrawStartegy());

    return flower;
}

std::unique_ptr<sas::Plant> sas::Weed::createOffspring(const Position &p) const noexcept
{
    std::unique_ptr<Weed> weed = std::make_unique<Weed>(p, getDrawStartegy());

    return weed;
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
