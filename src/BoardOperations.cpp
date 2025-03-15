#include "../include/BoardOperations.hpp"
#include "Common.hpp"
#include "Grid.hpp"
#include "Utils.hpp"
#include <deque>
#include <random>
#include "BoardOperations.hpp"
#include "Utils.hpp"

#include "../Extern/FastNoiseLite.h"

// The grid does NOT know about the TILES!!
// Not necesarry a bad thing
void sas::SetUpBoard(sas::Matrix<sas::Tile> &board)
{

    for (size_t i = 0; i < WidthCells; ++i)
    {
        for (size_t j = 0; j < HeightCells; ++j)
        {
            if (j <= 1.f * WidthCells / 6 || j > 5.f * WidthCells / 6)
            {
                board(i, j).type = sas::TileType::SNOW;
                board(i, j).setDrawStrategy(std::make_unique<sas::SnowDrawStrategy>());
            }
            else if (j <= 5.f * WidthCells / 12 || j > 7.f * WidthCells / 12)
            {
                board(i, j).type = sas::TileType::GRASS;
                board(i, j).setDrawStrategy(std::make_unique<sas::GrassDrawStrategy>());
            }
            else
            {
                board(i, j).type = sas::TileType::DESERT;
                board(i, j).setDrawStrategy(std::make_unique<sas::DesertDrawStrategy>());
            }

            board(i, j).pos = {i * cellSize, j * cellSize, static_cast<size_t>(cellSize), static_cast<size_t>(cellSize)};
        }
    }
}

void sas::SetUpBoardPerlin(Matrix<Tile> &board)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(std::random_device{}());

    constexpr float scale = 10; // Controls biome size (lower = bigger biomes)

    for (size_t i = 0; i < WidthCells; ++i)
    {
        for (size_t j = 0; j < HeightCells; ++j)
        {
            float noiseValue = noise.GetNoise((float)i * scale, (float)j * scale);
            noiseValue = (noiseValue + 1) / 2.f;

            std::print("Noise value for ({}, {}) is: {}\n", i, j, noiseValue);

            if (noiseValue < 0.3f)
            {
                board(i, j).type = sas::TileType::SNOW;
                board(i, j).setDrawStrategy(std::make_unique<sas::SnowDrawStrategy>());
            }
            else if (noiseValue < 0.6f)
            {
                board(i, j).type = sas::TileType::GRASS;
                board(i, j).setDrawStrategy(std::make_unique<sas::GrassDrawStrategy>());
            }
            else
            {
                board(i, j).type = sas::TileType::DESERT;
                board(i, j).setDrawStrategy(std::make_unique<sas::DesertDrawStrategy>());
            }

            board(i, j).pos = {i * cellSize, j * cellSize, static_cast<size_t>(cellSize), static_cast<size_t>(cellSize)};
        }
    }
}
void sas::SetUpWater(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::Grid &grid) noexcept
{
    // Random ahh value for wata
    for (size_t i = 0; i < 100; ++i)
    {
        const auto [x, y] = sas::generateNextPos();
        waters.push_back(sas::enviromentFactory(grid, {x, y}, sas::EnviromentType::WATER, std::make_unique<sas::WaterDrawStrategy>()));
    }
}

void sas::SetUpWaterNoise(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::Grid &grid, size_t seed) noexcept
{
    FastNoiseLite perlin;
    perlin.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    perlin.SetSeed(seed);

    FastNoiseLite worley;
    worley.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    worley.SetSeed(seed);

    constexpr float scaleElevation = 8; // Controls biome size (lower = bigger biomes)
    constexpr float scaleLakes = 8; // Controls biome size (lower = bigger biomes)
    constexpr float scaleVally = 10; // Controls biome size (lower = bigger biomes)

    for(size_t i = 0; i < WidthCells; ++i)
    {
        for(size_t j = 0; j < HeightCells; ++j)
        {
            float elevation = (perlin.GetNoise(i * scaleElevation, j * scaleElevation) + 1.f) * 0.5;
            float lakes = (worley.GetNoise(i * scaleLakes, j * scaleLakes) + 1.0f) * 0.5f;
            float valleys = 1.0f - abs(perlin.GetNoise(i  * scaleVally, j * scaleVally) + 1.f) * 0.5f;


            std::print("Elevation = {}, lake = {}, vally = {}\n", elevation, lakes, valleys);

            if (elevation < 0.3f || lakes < 0.25f /* || valleys > 0.6f */ )
            {
                waters.push_back(sas::enviromentFactory(grid, {i * cellSize, j * cellSize, cellSize, cellSize}, sas::EnviromentType::WATER, std::make_unique<sas::WaterDrawStrategy>()));
            }
        }
    }
}

void sas::spawnWeed(sas::Grid &grid, const sas::Matrix<sas::Tile> &board, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept
{
    const auto &elems = sas::chooseWeedCoords(board);

    std::vector<std::unique_ptr<sas::Plant>> newPlants;
    // Assume all elements have place
    newPlants.reserve(elems.size());

    // static to initialize only once
    static sas::Weed defaultWeed(0, 0, nullptr);

    for (const auto &sp : elems)
    {
        bool canPlant = true;

        const auto &neighbours = sas::findNearbyEntities<sas::Plant>(grid, sp.first, sp.second, defaultWeed.size);

        for (const auto &neighbour : neighbours)
        {
            if (!sas::checkBoundaries(sp, defaultWeed.size, neighbour->getPosition(), defaultWeed.size)) // am zis 17 ca 10sqrt(2) pt diagonala unui bloc de apa
            {                                                                                            // not proud of this one tbh
                canPlant = false;
                break;
            }
        }

        if (canPlant)
        {
            newPlants.push_back(sas::plantFactory(grid, sp.first, sp.second, sas::PlantType::WEED, std::make_shared<sas::WeedDrawStrategy>()));
        }
    }

    std::move(newPlants.begin(), newPlants.end(), std::back_inserter(plants));
}

void sas::multiply(sas::Grid &grid, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept
{
    static std::deque<std::unique_ptr<sas::Plant>> newPlants;
    newPlants.clear();

    for (const auto &plt : plants)
    {
        const auto &spawnPoints = plt->reproduce();
        for (const auto &sp : spawnPoints)
        {
            bool canPlant = true;

            auto waterSource = findNearestEntity<Water>(grid, sp.first, sp.second, plt->rangeWater,
                                                                  [&](Water &wat)
                                                                  {
                                                                    return (wat.capacity >= plt->getWaterConsumption());
                                                                  });

            if (waterSource == nullptr)
            {
                continue;
            }

            const auto &neighbours = findNearbyEntities<Plant>(grid, sp.first, sp.second, plt->size);

            for (const auto &neighbour : neighbours)
            {
                if (!sas::checkBoundaries(sp, 17, neighbour->getPosition(), 17)) // am zis 17 ca 10sqrt(2) pt diagonala unui bloc de apa
                {                                                                // not proud of this one tbh
                    canPlant = false;
                    break;
                }
            }

            if (canPlant)
            {
                newPlants.push_back(sas::plantFactory(grid, sp.first, sp.second, plt->getPlantType(), plt->getDrawStartegy()));
                waterSource->capacity = waterSource->capacity - plt->getWaterConsumption();
            }
        }
    }

    std::move(newPlants.begin(), newPlants.end(), std::back_inserter(plants));
}

void sas::killPlants(sas::Grid &grid, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept
{
    for (size_t i = 0; i < plants.size();)
    {
        // O(1) removal
        if (plants[i]->willWither())
        {
            // Remember this could return nullptr
            auto closestWater = findNearestEntity<sas::Water>(grid, plants[i]->pos.x, plants[i]->pos.y, plants[i]->rangeWater);
            if (closestWater)
            {
                closestWater->capacity = closestWater->capacity + plants[i]->waterConsumption;
            }
            sas::removeFromGrid(grid, plants.at(i).get());
            std::swap(plants[i], plants.back());
            plants.pop_back();
        }
        else
        {
            ++i;
        }
    }
}