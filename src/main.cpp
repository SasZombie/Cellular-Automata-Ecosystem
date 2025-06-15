#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <deque>
#include <iostream>
#include <print>
#include <chrono>
#include "../include/Matrix.hpp"
#include "../include/Tile.hpp"
#include "../include/Grid.hpp"
#include "../include/Utils.hpp"
#include "../include/Common.hpp"
#include "../include/BoardOperations.hpp"
#include "../include/ConfigManager.hpp"

static void handleCameraControlls(Camera2D &camera)
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

/// @brief
// board = background --> Fastest
// enviroment = stuff that has collision such as water, needs a static grid --> Not as fast
// plant/rest = stuff that has collision but not necesarrly cellSize x cellSize, needs dynamic grid --> still fast but slower than the others
// This ^ also takes into account multiple entities on the same cell: 2 plants on the same cell, extra memory for this

int main()
{

    // size_t seed = sas::generateSeed();
    size_t seed = 18;
    // All sizes are in cells
    sas::ConfigManager::load("config.json");

    sas::Matrix<sas::Tile> board(WidthCells, HeightCells);
    sas::SetUpBoardPerlin(board, seed);

    // Since the grid only knows about some positions
    // We need to talk about different Enviroments
    std::vector<std::unique_ptr<sas::Enviroment>> enviroment;
    sas::StaticGrid enviromentGrid;
    sas::SetUpWaterNoise(enviroment, enviromentGrid, seed);

    std::vector<std::unique_ptr<sas::Plant>> plants;
    sas::DynamicGrid plantGrid;
    std::unique_ptr<sas::Flower> flower = std::make_unique<sas::Flower>(sas::Position{40, 40, 20, 20}, std::make_unique<sas::PlaceholderDrawStrategy>());
    
    //Primul index de la Enviroment
    flower->waterSourceIndex = 0;
    sas::addPlant(std::move(flower), plantGrid, plants);

    Camera2D camera;
    camera.target = {0.f, 0.f};
    camera.offset = {0.f, 0.f};
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    constexpr size_t FPS = 60;
    InitWindow(ScreenWidth, ScreenHeight, "Celular Automata Ecosystem");
    SetTargetFPS(FPS);

    // Tick = day
    // Season = ??
    float tickAcc = 0.f;
    float seasonAcc = 0.f;
    constexpr float intervalTick = 10.f / FPS;
    constexpr float intervalSeason = 16 * intervalTick;
    const std::string text = "Plus si minus pt zoom\nWASD pentru movement Seed = " + std::to_string(seed);

    while (!WindowShouldClose())
    {

// On by default
#ifdef ENABLE_CONFIG_RELOAD
        if (IsKeyPressed(KEY_L))
        {
            // TODO:
            //  reloadConfig();
        }

#endif

        float dt = GetFrameTime();

        tickAcc = tickAcc + dt;

        if (tickAcc >= intervalTick)
        {
            tickAcc = 0;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            sas::multiplyPlants(plantGrid, enviromentGrid, enviroment, plants);
            // sas::spawnWeed(grid, board, plants);

            std::for_each(plants.begin(), plants.end(), [](std::unique_ptr<sas::Plant> &plt)
                          { plt->daysAlive++; });

            sas::killPlants(plantGrid, plants, enviroment);
        }

        if (IsKeyPressed(KEY_R))
        {
            sas::SetUpBoardPerlin(board, seed);
        }

        if (IsKeyPressed(KEY_Q))
        {
            for (const auto &plt : plants)
            {
                std::print("Position {}, {}, {}, {}\n", plt->pos.x, plt->pos.y, plt->pos.width, plt->pos.height);
            }
        }
#if 0
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            const auto [x, y] = GetMousePosition();

            const float cx = camera.target.x, cy = camera.target.y;

            // TODO: Make zoom work...
            const float cz = camera.zoom;
            sas::Position flower{(size_t)x, (size_t)y, 20, 20};

            if (sas::isNearWater(flower, enviromentGrid, sas::ConfigManager::get().Flower.WaterRange))
            {
                size_t spatialCellSize = sas::spatialCellSize;

                
                std::unique_ptr<sas::Flower> newPlant = std::make_unique<sas::Flower>(flower, std::make_unique<sas::PlaceholderDrawStrategy>());
                plants.push_back(std::move(newPlant));

                int left = flower.x / spatialCellSize;
                int right = (flower.x + flower.width) / spatialCellSize;
                int top = flower.y / spatialCellSize;
                int bottom = (flower.y + flower.height) / spatialCellSize;

                for (int gx = left; gx <= right; ++gx)
                {
                    for (int gy = top; gy <= bottom; ++gy)
                    {
                        plantGrid[{gx, gy}].push_back(plants.size() - 1);
                    }
                }
            }

            // for (const auto &elem : elems)
            // {
            // std::print("Found element at pos {{{}, {}}}, mouse is at coords({}, {})\n", elem->pos.x, elem->pos.y, x, y);
            // }
        }
#endif
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

        DrawText(text.c_str(), 0, 0, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
}