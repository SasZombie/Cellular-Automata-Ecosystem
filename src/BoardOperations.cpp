#include "../include/BoardOperations.hpp"
#include "ConfigManager.hpp"
#include "Common.hpp"
#include "Grid.hpp"
#include "Utils.hpp"
#include <deque>
#include <random>
#include <iostream>
#include "BoardOperations.hpp"
#include <raylib.h>
#include "Utils.hpp"
#include <assert.h>
#include <queue>
#include <unordered_set>


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

void sas::SetUpBoardPerlin(Matrix<Tile> &board, size_t seed)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(seed);

    constexpr float scale = 10; // Controls biome size (lower = bigger biomes)

    for (size_t i = 0; i < WidthCells; ++i)
    {
        for (size_t j = 0; j < HeightCells; ++j)
        {
            float noiseValue = noise.GetNoise((float)i * scale, (float)j * scale);
            noiseValue = (noiseValue + 1) / 2.f;

            // std::print("Noise value for ({}, {}) is: {}\n", i, j, noiseValue);

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
void sas::SetUpWater(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::StaticGrid &grid) noexcept
{
    // Random ahh value for wata
    // waters.push_back(sas::enviromentFactory(grid, {40, 40, 20, 20}, sas::EnviromentType::WATER, std::make_unique<sas::WaterDrawStrategy>()));
}

void sas::SetUpWaterNoise(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::StaticGrid &grid, size_t seed) noexcept
{
    FastNoiseLite perlin;
    perlin.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    perlin.SetSeed(seed);

    FastNoiseLite worley;
    worley.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    worley.SetSeed(seed);

    constexpr float scaleElevation = 8; // Controls biome size (lower = bigger biomes)
    constexpr float scaleLakes = 8;     // Controls biome size (lower = bigger biomes)
    constexpr float scaleVally = 10;    // Controls biome size (lower = bigger biomes)

    for (size_t i = 0; i < WidthCells; ++i)
    {
        for (size_t j = 0; j < HeightCells; ++j)
        {
            float elevation = (perlin.GetNoise(i * scaleElevation, j * scaleElevation) + 1.f) * 0.5;
            float lakes = (worley.GetNoise(i * scaleLakes, j * scaleLakes) + 1.0f) * 0.5f;
            float valleys = 1.0f - abs(perlin.GetNoise(i * scaleVally, j * scaleVally) + 1.f) * 0.5f;

            // std::print("Elevation = {}, lake = {}, vally = {}\n", elevation, lakes, valleys);

            if (elevation < 0.3f || lakes < 0.25f /* || valleys > 0.6f */)
            {
                addWater(i, j, waters, grid);
            }
        }
    }
}

void sas::spawnWeed(sas::DynamicGrid &plantGrid, sas::StaticGrid &enviromentGrid, std::vector<std::unique_ptr<sas::Enviroment>> &water, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept
{

    const static int rangeWater = ConfigManager::get().Weed.WaterRange;
    const auto &points = chooseWeedCoords();
    for (const auto &sp : points)
    {
        if (hasSpaceAgainstPlants(sp, plants, plantGrid, enviromentGrid))
        {
            std::optional<size_t> result = getClosestWaterCell(sp, enviromentGrid, water, rangeWater);
            if (result)
            {
                std::unique_ptr<sas::Weed> newPlant = std::make_unique<Weed>(sp, std::make_unique<WeedDrawStrategy>());
                water[*result]->capacity -= newPlant->waterConsumption;
                newPlant->waterSourceIndex = *result;
                addPlant(std::move(newPlant), plantGrid, plants);
            }
        }
    }
}


bool sas::hasSpaceAgainstPlants(const Position &p, const std::vector<std::unique_ptr<Plant>> &plants, const DynamicGrid &plantGrid, const StaticGrid &envGrid) noexcept
{
    return !collides(p, envGrid) && !collides(p, plants, plantGrid);
}

void sas::addPlant(std::unique_ptr<sas::Plant> plant, sas::DynamicGrid &plantGrid, std::vector<std::unique_ptr<sas::Plant>> &plants, int spatialCellSize)
{

    int left = plant->pos.x / spatialCellSize;
    int right = (plant->pos.x + plant->pos.width) / spatialCellSize;
    int top = plant->pos.y / spatialCellSize;
    int bottom = (plant->pos.y + plant->pos.height) / spatialCellSize;

    
    for (int gx = left; gx <= right; ++gx)
    {
        for (int gy = top; gy <= bottom; ++gy)
        {
            plantGrid[{gx, gy}].push_back(plant.get());
        }
    }
    plants.push_back(std::move(plant));
}

// TODO: Do this with threads (LOL)
void sas::multiplyPlants(sas::DynamicGrid &plantGrid, sas::StaticGrid &enviromentGrid, std::vector<std::unique_ptr<sas::Enviroment>> &water, std::vector<std::unique_ptr<sas::Plant>> &plants) noexcept
{
    size_t realSize = plants.size();

    for (size_t i = 0; i < realSize; ++i)
    {
        // Absolutlly under no circumstance do this:
        // const auto &plt = plants[i];
        // This ^ will be invalidated when the vector reallocs!!!
        const auto &points = plants[i]->reproduce();
        for (const auto &sp : points)
        {
            if (hasSpaceAgainstPlants(sp, plants, plantGrid, enviromentGrid))
            {
                std::optional<size_t> result = getClosestWaterCell(sp, enviromentGrid, water, static_cast<int>(plants[i]->rangeWater));
                if (result)
                {
                    std::unique_ptr<sas::Plant> newPlant = plants[i]->createOffspring(sp);
                    water[*result]->capacity -= newPlant->waterConsumption;
                    newPlant->waterSourceIndex = *result;
                    addPlant(std::move(newPlant), plantGrid, plants);
                }
            }
        }
    }
}

std::optional<size_t> sas::isNearWater(const Position &pos, const StaticGrid &waterCells, std::vector<std::unique_ptr<sas::Enviroment>> &water, const Plant *p) noexcept
{
    int centerX = (pos.x + pos.width / 2) / cellSize;
    int centerY = (pos.y + pos.height / 2) / cellSize;
    int maxRange = p->rangeWater;

    for (int dx = -maxRange; dx <= maxRange; ++dx)
    {
        for (int dy = -maxRange; dy <= maxRange; ++dy)
        {
            if (abs(dx) + abs(dy) > maxRange)
                continue;

            GridPos neighbor = {centerX + dx, centerY + dy};
            auto it = waterCells.find(neighbor);

            if (it != waterCells.end())
            {
                size_t index = it->second;
                if (water[index]->capacity >= p->getWaterConsumption())
                {
                    return index;
                }
            }
        }
    }

    return std::nullopt;
}

void sas::addWater(int gridX, int gridY, std::vector<std::unique_ptr<sas::Enviroment>> &water, sas::StaticGrid &waterCells) noexcept
{
    std::unique_ptr<sas::Water> env = std::make_unique<sas::Water>();

    env->pos = {static_cast<size_t>(gridX) * cellSize, static_cast<size_t>(gridY) * cellSize, cellSize, cellSize};
    env->setDrawStrategy(std::make_unique<sas::WaterDrawStrategy>());

    water.push_back(std::move(env));
    waterCells[{gridX, gridY}] = water.size() - 1;
}

void sas::killPlants(sas::DynamicGrid &plantGrid, std::vector<std::unique_ptr<sas::Plant>> &plants, std::vector<std::unique_ptr<sas::Enviroment>> &water) noexcept
{
    for (size_t i = 0; i < plants.size();)
    {
        // O(1) removal
        if (plants[i]->willWither())
        {
            //No checking if water source is not nullptr...
            //It should NEVER be nullptr, but if we encounter crashes
            //We should add the check here to see if it solves it
            water[plants[i]->waterSourceIndex]->capacity += plants[i]->waterConsumption;
            
            sas::removeFromGrid(plantGrid, plants[i].get());
            std::swap(plants[i], plants.back());
            plants.pop_back();
        }
        else
        {
            ++i;
        }
    }
}

std::optional<size_t> sas::getClosestWaterCell(const Position &pos, const StaticGrid &waterCells, std::vector<std::unique_ptr<sas::Enviroment>> &water, int maxRange) noexcept
{
    int gridX = pos.x / cellSize, gridY = pos.y / cellSize;
    std::queue<GridPos> q;
    std::unordered_set<GridPos, PairHash> visited;

    q.push({gridX, gridY});
    visited.insert({gridX, gridY});

    const std::vector<GridPos> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1} 
    };

    while (!q.empty())
    {
        auto [x, y] = q.front();
        q.pop();

        if (waterCells.contains({x, y}))
        {
            return {waterCells.at({x, y})};
        }

        for (const auto &[dx, dy] : directions)
        {
            int nx = x + dx;
            int ny = y + dy;

            //Respect maxRange
            //Chebyshev Distance
            if (std::max(std::abs(nx - gridX), std::abs(ny - gridY)) > maxRange)
                continue;

            GridPos next = {nx, ny};

            if (visited.count(next))
                continue;
            visited.insert(next);
            q.push(next);
        }
    }

    // No water found within maxRange
    return std::nullopt;
}