#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>
#include <print>
#include "../include/Matrix.hpp"
#include "../include/Tile.hpp"
#include "../include/Grid.hpp"
#include "../include/Utils.hpp"
#include "../include/Common.hpp"

// The grid DOES NOT know about the TILES!!
// Not necesarry a bad thing
void SetUpBoard(sas::Matrix<sas::Tile> &board)
{

    for (size_t i = 0; i < WidthCells; ++i)
    {
        for (size_t j = 0; j < WidthCells; ++j)
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

void SetUpWater(std::vector<std::shared_ptr<sas::Enviroment>> &waters, sas::Grid &grid)
{
    // Random ahh value for wata
    for (size_t i = 0; i < 100; ++i)
    {
        const auto [x, y] = sas::generateNextPos();
        waters.push_back(sas::enviromentFactory(grid, {x, y}, sas::EnviromentType::WATER, std::make_unique<sas::WaterDrawStrategy>()));
    }
}

void spawnWeed(sas::Grid &grid, const sas::Matrix<sas::Tile> &board, std::vector<std::shared_ptr<sas::Plant>> &plants)
{
    const auto &elems = sas::chooseWeedCoords(board);

    std::vector<std::shared_ptr<sas::Plant>> newPlants;
    // Assume all plants reproduce
    newPlants.reserve(plants.size());

    for (const auto &sp : elems)
    {
        bool canPlant = true;


        //HardCoded...
        const auto &neighbours = sas::findNearbyEntities<sas::Plant>(grid, sp.first, sp.second, 10);

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
            newPlants.push_back(sas::plantFactory(grid, sp.first, sp.second, sas::PlantType::WEED, std::make_shared<sas::WeedDrawStrategy>()));
        }
    }

    std::move(newPlants.begin(), newPlants.end(), std::back_inserter(plants));
}

void multiply(sas::Grid &grid, std::vector<std::shared_ptr<sas::Plant>> &plants)
{
    std::vector<std::shared_ptr<sas::Plant>> newPlants;
    // Assume all plants reproduce
    newPlants.reserve(plants.size());

    // Idea: Do this with threads!
    for (const auto &plt : plants)
    {
        const auto &spawnPoints = plt->reproduce();
        for (const auto &sp : spawnPoints)
        {
            bool canPlant = true;

            auto waterSource = sas::findNearestEntity<sas::Water>(grid, sp.first, sp.second, plt->rangeWater,
                                                                  [&](sas::Water &wat)
                                                                  {
                                                                      return (wat.capacity >= plt->waterConsumption());
                                                                  });

            if (waterSource == nullptr)
            {
                continue;
            }

            const auto &neighbours = sas::findNearbyEntities<sas::Plant>(grid, sp.first, sp.second, plt->size);

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
                waterSource->capacity = waterSource->capacity - plt->waterConsumption();
            }
        }
    }

    std::move(newPlants.begin(), newPlants.end(), std::back_inserter(plants));
}

void handleCameraControlls(Camera2D &camera)
{
    if (IsKeyDown(KEY_D))
        camera.target.x += 2;
    else if (IsKeyDown(KEY_A) && camera.target.x > 0)
        camera.target.x -= 2;
    if (IsKeyDown(KEY_W) && camera.target.y > 0)
        camera.target.y -= 2;
    else if (IsKeyDown(KEY_S))
        camera.target.y += 2;

    // Camera zoom controls
    if (IsKeyDown(KEY_ZERO))
        camera.zoom = -1.f;
    if (IsKeyDown(KEY_MINUS))
        camera.zoom += 0.005f;
    else if (IsKeyDown(KEY_EQUAL))
        camera.zoom -= 0.005f;

    camera.zoom += ((float)GetMouseWheelMove() * -0.05f);

    // Camera reset (zoom and rotation)
    if (IsKeyPressed(KEY_R))
    {
        camera.zoom = 1.0f;
        camera.rotation = 0.0f;
    }
}

int main()
{

    size_t seed = sas::generateSeed();

    sas::Grid grid;
    std::vector<std::shared_ptr<sas::Plant>> plants;
    std::vector<std::shared_ptr<sas::Enviroment>> enviroment;
    // This uses std::vector underneath but when talking about tiles
    // It is easier to think about them based on a matrix
    sas::Matrix<sas::Tile> board(WidthCells, HeightCells);

    plants.push_back(sas::plantFactory(grid, 5, 5, sas::PlantType::FLOWER, std::make_unique<sas::FlowerDrawStrategy>()));
    plants.push_back(sas::plantFactory(grid, 100, 100, sas::PlantType::TREE, std::make_unique<sas::TreeDrawStrategy>()));
    enviroment.push_back(sas::enviromentFactory(grid, 7, 7, sas::EnviromentType::WATER, std::make_unique<sas::WaterDrawStrategy>()));

    SetUpBoard(board);
    SetUpWater(enviroment, grid);

    Camera2D camera;
    camera.target = {0.f, 0.f};
    camera.offset = {0.f, 0.f};
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    constexpr size_t FPS = 60;
    InitWindow(ScreenWidth, ScreenHeight, "Celular Automata Ecosystem");
    SetTargetFPS(FPS);

    float timeAcc = 0.f;
    constexpr float interval = 10.f / FPS;
    const std::string text = "Plus si minus pt zoom\nWASD pentru movement Seed = " + std::to_string(seed);

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();

        timeAcc = timeAcc + dt;

        if (timeAcc >= interval)
        {
            timeAcc = 0;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            multiply(grid, plants);
            spawnWeed(grid, board, plants);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            const auto [x, y] = GetMousePosition();

            const float cx = camera.target.x, cy = camera.target.y;

            // TODO: Make zoom work...
            const float cz = camera.zoom;
            const auto &elems = sas::findNearbyEntities<sas::Flower>(grid, static_cast<size_t>(x), static_cast<size_t>(y), 50);

            for (const auto &elem : elems)
            {
                std::print("Found element at pos {{{}, {}}}\n", elem->pos.x, elem->pos.y);
            }
        }

        BeginDrawing();

        ClearBackground(Color{18, 18, 18, 255});

        BeginMode2D(camera);

        handleCameraControlls(camera);

        for (const auto &tile : board)
        {
            tile.draw();
        }

        for (const auto &env : enviroment)
        {
            env->draw();
        }

        for (const auto &plt : plants)
        {
            plt->draw();
        }

        DrawText(text.c_str(), -20, -60, 20, WHITE);

        EndDrawing();
    }
    CloseWindow();
}