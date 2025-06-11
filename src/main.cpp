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

void AddWater(int gridX, int gridY, std::vector<std::unique_ptr<sas::Enviroment>> &water, sas::StaticGrid &waterCells)
{
    std::unique_ptr<sas::Water> env = std::make_unique<sas::Water>();

    env->pos = {static_cast<size_t>(gridX) * cellSize, static_cast<size_t>(gridY) * cellSize, cellSize, cellSize};
    env->setDrawStrategy(std::make_unique<sas::WaterDrawStrategy>());

    water.push_back(std::move(env));
    waterCells.insert({gridX, gridY});
}

int main()
{

    // size_t seed = sas::generateSeed();
    size_t seed = 18;
    sas::ConfigManager::load("config.json");

    sas::Matrix<sas::Tile> board(WidthCells, HeightCells);
    sas::SetUpBoard(board);

    std::vector<std::unique_ptr<sas::Enviroment>> enviroment;
    sas::StaticGrid enviromentGrid;
    AddWater(0, 0, enviroment, enviromentGrid);
    AddWater(0, 1, enviroment, enviromentGrid);
    AddWater(1, 2, enviroment, enviromentGrid);

    std::vector<std::unique_ptr<sas::Plant>> plants;
    sas::DynamicGrid plantGrid;
    std::unique_ptr<sas::Flower> flower = std::make_unique<sas::Flower>(sas::Position{40, 40, 20, 20}, std::make_unique<sas::PlaceholderDrawStrategy>());

    sas::AddPlant(std::move(flower), plantGrid, plants);

    // std::unique_ptr
    // plants.push_back();

    // This uses std::vector underneath but when talking about tiles
    // It is easier to think about them based on a matrix

    // plants.push_back(sas::plantFactory(grid, 40, 20, sas::PlantType::FLOWER, std::make_unique<sas::FlowerDrawStrategy>()));
    // plants.push_back(sas::plantFactory(grid, 150, 150, sas::PlantType::TREE, std::make_unique<sas::TreeDrawStrategy>()));

    // TODO!!
    // TREBUIE NEAPARAT DISCUTAT CU COSMIN DESPRE CUM FACEM ACCEEST PROIECT
    // ESTE ESSENTIAL SA VEDEM DACA ARE SENS SA CONTINUAM CU GRIDUL ASTA ASA
    // SAU PUR SI SIMPLU INGORAM ASTA
    //  sas::SetUpBoardPerlin(board, seed);
    //  sas::SetUpWaterNoise(enviroment, grid, seed);

    Camera2D camera;
    camera.target = {0.f, 0.f};
    camera.offset = {0.f, 0.f};
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    constexpr size_t FPS = 60;
    InitWindow(ScreenWidth, ScreenHeight, "Celular Automata Ecosystem");
    SetTargetFPS(FPS);

    // Tick = day
    // Season =
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
            // reloadConfig();
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
            sas::multiplyPlants(plantGrid, enviromentGrid, plants);
            // sas::spawnWeed(grid, board, plants);

            // std::for_each(plants.begin(), plants.end(), [](std::unique_ptr<sas::Plant> &plt)
            //               { plt->daysAlive++; });

            // sas::killPlants(grid, plants);
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

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            const auto [x, y] = GetMousePosition();

            const float cx = camera.target.x, cy = camera.target.y;

            // TODO: Make zoom work...
            const float cz = camera.zoom;

            if (!sas::collides(sas::Position{(size_t)x, (size_t)y, 20, 20}, plants, plantGrid))
            {
                size_t spatialCellSize = sas::spatialCellSize;

                sas::Position flower{(size_t)x, (size_t)y, 20, 20};
                
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

        BeginDrawing();

        ClearBackground(Color{18, 18, 18, 255});

        BeginMode2D(camera);

        handleCameraControlls(camera);

        for (const auto &tile : board)
        {
            tile.draw();
            // const auto [x, y, w, h] = tile.pos;
            // DrawText((std::to_string(x) + ' ' + std::to_string(y)).c_str(), x, y, 10, RAYWHITE);
        }

        for (const auto &env : enviroment)
        {
            env->draw();
            // const auto [x, y, w, h] = env->pos;

            // DrawText((std::to_string(x) + ' ' + std::to_string(y)).c_str(), x, y, 10, RAYWHITE);
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