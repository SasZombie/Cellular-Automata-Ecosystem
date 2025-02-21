#include "../include/Plant.hpp"
#include <random>
#include <iostream>

static constexpr float TreeReproductionChance = 0.02f;
static constexpr float FlowerReproductionChance = 0.2f;
static constexpr float WeedReproductionChance = 0.06f;

static constexpr size_t WaterConsumptionTree = 10;
static constexpr size_t WaterConsumptionFlower = 5;
static constexpr size_t WaterConsumptionWeed = 30;

static std::random_device rd;
static std::mt19937 gen;
static std::uniform_real_distribution<float> dist;

bool sas::Tree::reproduce() const noexcept
{
    return dist(gen) <= TreeReproductionChance;
}

bool sas::Weed::reproduce() const noexcept
{
    return dist(gen) <= WeedReproductionChance;
}

bool sas::Flower::reproduce() const noexcept
{
    return dist(gen) <= FlowerReproductionChance;
}

size_t sas::Flower::waterConsumption() const noexcept
{
    std::cout << "Flower";
    return 0;
}

size_t sas::Tree::waterConsumption() const noexcept
{
    std::cout << "Tree";

    return 0;
}

size_t sas::Weed::waterConsumption() const noexcept
{
    std::cout << "Weed";

    return 0;
}