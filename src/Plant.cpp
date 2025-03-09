#include "../include/Plant.hpp"
#include <random>
#include <print>
#include "Utils.hpp"
#include <iostream>
#include "../include/Utils.hpp"
#include "raylib.h"

static constexpr float TreeReproductionChance = 0.02f;
static constexpr float FlowerReproductionChance = 0.2f;
static constexpr float WeedReproductionChance = 0.06f;

static constexpr size_t WaterConsumptionTree = 10;
static constexpr size_t WaterConsumptionFlower = 5;
static constexpr size_t WaterConsumptionWeed = 30;


std::vector<std::pair<size_t, size_t>> sas::Plant::reproduce() const noexcept
{

    std::vector<std::pair<size_t, size_t>> vecs;


    for(size_t i = 0; i < this->nrOfSeeds; ++i)
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