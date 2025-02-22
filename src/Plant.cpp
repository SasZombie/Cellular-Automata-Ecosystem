#include "../include/Plant.hpp"
#include <random>
#include <iostream>
#include "raylib.h"

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
// TODO: return value
size_t sas::Flower::waterConsumption() const noexcept
{
    std::cout << "Flower";
    return 0;
}
// TODO: return value
size_t sas::Tree::waterConsumption() const noexcept
{
    std::cout << "Tree";

    return 0;
}
// TODO: return value
size_t sas::Weed::waterConsumption() const noexcept
{
    std::cout << "Weed";

    return 0;
}

void sas::Tree::draw(int x, int y) const noexcept
{
    size_t plantSize = 20; // Impartire la 2
    DrawCircle(plantSize << 1, plantSize << 1, plantSize, LIME);
}

void sas::Weed::draw(int x, int y) const noexcept
{
    size_t plantSize = 8; // Impartire la 2
    DrawCircle(plantSize << 1, plantSize << 1, plantSize, Color{80, 150, 80, 255});
}

void sas::Flower::draw(int x, int y) const noexcept
{
    size_t plantSize = 8; // Impartire la 2
    DrawCircle(plantSize << 1, plantSize << 1, plantSize, PINK);
}