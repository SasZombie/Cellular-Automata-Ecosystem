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
    size_t plantSize = 10; // Impartire la 2
    DrawCircle(x, y, plantSize, Color{30, 215, 80, 255});
    DrawCircle(x - plantSize / 3    , y - (plantSize >> 1)  , 3 * plantSize >> 2, Color{60, 175, 20, 255});
    DrawCircle(x                    , y + 2 * plantSize / 3 , 3 * plantSize >> 2, Color{0, 105, 0, 255});
    DrawCircle(x + (plantSize >> 1) , y                     , 3 * plantSize >> 2, Color{110, 180, 0, 255});
}

void sas::Weed::draw(int x, int y) const noexcept
{
    size_t plantSize = 5; // Impartire la 2
    float fx = static_cast<float>(x), fy = static_cast<float>(y);

    DrawTriangle({fx - plantSize, fy - plantSize}, {fx - plantSize, fy + plantSize}, {fx, fy}, Color{80, 150, 80, 255});
    DrawTriangle({fx + plantSize, fy + plantSize}, {fx + plantSize, fy - plantSize}, {fx, fy}, Color{80, 150, 80, 255});
}

void sas::Flower::draw(int x, int y) const noexcept
{
    size_t plantSize = 3;
    float fx = static_cast<float>(x), fy = static_cast<float>(y);

    // DrawTriangle({fx, fy}, {fx, fy - plantSize}, {fx - plantSize, fy}, PINK); // TopLeft
    // DrawTriangle({fx, fy}, {fx - plantSize, fy}, {fx, fy + plantSize}, PINK); // BottomLeft
    // DrawTriangle({fx, fy}, {fx, fy + plantSize}, {fx + plantSize, fy}, PINK); // BottomRight
    // DrawTriangle({fx, fy}, {fx + plantSize, fy}, {fx, fy - plantSize}, PINK); // TopRight

    DrawTriangle({fx, fy + 2 * plantSize}, {fx, fy - 2 * plantSize}, {fx - 2 * plantSize, fy}, LIME); // LeftPart
    DrawTriangle({fx, fy + 2 * plantSize}, {fx + 2 * plantSize, fy}, {fx, fy - 2 * plantSize}, LIME); // Right
    DrawCircle(x - plantSize, y - plantSize, plantSize, PINK);
    DrawCircle(x - plantSize, y + plantSize, plantSize, PINK);
    DrawCircle(x + plantSize, y + plantSize, plantSize, PINK);
    DrawCircle(x + plantSize, y - plantSize, plantSize, PINK);
    DrawCircle(x, y, plantSize, RED);
    
    // DrawCircle(x, y, plantSize, PINK);
    // DrawLine(x - plantSize, y            , x + plantSize, y            , Color{150, 0, 100, 255});
    // DrawLine(x            , y - plantSize, x            , y + plantSize, Color{150, 0, 100, 255});
    // DrawCircle(x, y, plantSize / 3, RED);
}