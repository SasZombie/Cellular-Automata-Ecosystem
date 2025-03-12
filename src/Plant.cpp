#include "../include/Plant.hpp"
#include <random>
#include <print>
#include "Utils.hpp"
#include <iostream>
#include "../include/Utils.hpp"
#include "raylib.h"
#include "Common.hpp"


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