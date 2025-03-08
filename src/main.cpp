#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>
#include <print>
#include "../include/matrix.hpp"
#include "../include/Tile.hpp"
#include "../include/Grid.hpp"
#include "../include/Utils.hpp"

constexpr size_t ScreenWidth = 800;
constexpr size_t ScreenHeight = 800;
constexpr float cellSize = 100;

constexpr size_t boardWidth = 600;
constexpr size_t boardHeight = 600;
// constexpr float boardPartSize = 8;
// constexpr size_t boardSideLength = boardPartSize * cellSize;
// constexpr size_t boardPartsHorizontally = ScreenWidth / boardSideLength;
// constexpr size_t boardPartsVertically = ScreenHeight / boardSideLength;

// void DrawBoard(const sas::Matrix<sas::Tile> &board);
// void SetUpBoard(sas::Matrix<sas::Enviroment> &board);

// The grid DOES NOT know about the TILES!!
void SetUpBoard(sas::Matrix<sas::Tile> &board)
{
    size_t tilePerWidth = ScreenWidth / cellSize, tilePerHeight = ScreenHeight / cellSize;

    for (size_t i = 0; i < ScreenWidth; i += cellSize)
    {
        for (size_t j = 0; j < ScreenHeight; j += cellSize)
        {
            if (j <= 1.f * ScreenWidth / 6 || j > 5.f * ScreenWidth / 6)
            {
                board(i, j).type = sas::TileType::SNOW;
                board(i, j).setDrawStrategy(std::make_unique<sas::SnowDrawStrategy>());
            }
            else if (j <= 5.f * ScreenWidth / 12 || j > 7.f * ScreenWidth / 12)
            {
                board(i, j).type = sas::TileType::GRASS;
                board(i, j).setDrawStrategy(std::make_unique<sas::GrassDrawStrategy>());
            }
            else
            {
                board(i, j).type = sas::TileType::DESERT;
                board(i, j).setDrawStrategy(std::make_unique<sas::DesertDrawStrategy>());
            }

            board(i, j).pos = {i, j, static_cast<size_t>(cellSize), static_cast<size_t>(cellSize)};
        }
    }
}

void SetUpWater(std::vector<std::unique_ptr<sas::Enviroment>> &waters, sas::Grid &grid)
{
    // Random ahh value for wata
    for (size_t i = 0; i < 10; ++i)
    {
        const auto [x, y] = sas::generateNextPos();
        waters.push_back(sas::enviromentFactory(grid, {x, y}, sas::EnviromentType::WATER, std::make_unique<sas::WaterDrawStrategy>()));
    }
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
    std::vector<std::unique_ptr<sas::Plant>> plants;
    std::vector<std::unique_ptr<sas::Enviroment>> enviroment;
    sas::Matrix<sas::Tile> board(ScreenWidth, ScreenHeight);

    plants.push_back(sas::plantFactory(grid, 100, 100, sas::PlatType::FLOWER, std::make_unique<sas::FlowerDrawStrategy>()));
    enviroment.push_back(sas::enviromentFactory(grid, 7, 7, sas::EnviromentType::WATER, std::make_unique<sas::WaterDrawStrategy>()));

    for (const auto &elem : grid)
    {
        std::print("Element in grid at pos{{{}, {}}}\n", elem.first.first, elem.first.second);
    }

    const auto &elems = sas::findNearbyEntities<sas::Flower>(grid, 100, 100, 200);

    for (const auto &elem : elems)
    {
        std::print("Found element at pos {{{}, {}}}", elem->pos.x, elem->pos.y);
    }

    return 1;
    SetUpBoard(board);
    SetUpWater(enviroment, grid);

    Camera2D camera;
    camera.target = {0.f, 0.f};
    camera.offset = {0.f, 0.f};
    // camera.offset = {ScreenWidth / 2.0f, ScreenHeight / 2.0f};
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
            std::vector<std::unique_ptr<sas::Plant>> newPlants;
            newPlants.reserve(plants.size());

            // Idea: Do this with threads!
            for (const auto &plt : plants)
            {
                const auto &spawnPoints = plt->reproduce();
                for (const auto &sp : spawnPoints)
                {
                    bool canPlant = true;

                    // auto waterSource = sas::findNearestEntity<sas::Water>(grid, sp.first, sp.second, plt->rangeWater,
                    //                                                       [&](sas::Water &wat)
                    //                                                       {
                    //                                                           return (wat.capacity >= plt->waterConsumption());
                    //                                                       });

                    // if (waterSource == nullptr)
                    // {
                    //     continue;
                    // }

                    const auto &neighbours = sas::findNearbyEntities<sas::Plant>(grid, sp.first, sp.second, plt->rangeWater);

                    for (const auto &neighbour : neighbours)
                    {
                        if (!sas::checkBoundaries(sp, 50, neighbour->getPosition(), 50)) // am zis 17 ca 10sqrt(2) pt diagonala unui bloc de apa
                        {                                                                // not proud of this one tbh
                            canPlant = false;
                            break;
                        }
                    }

                    if (canPlant)
                    {
                        newPlants.push_back(sas::plantFactory(grid, sp.first, sp.second, sas::PlatType::FLOWER, std::make_unique<sas::FlowerDrawStrategy>()));
                        std::cout << "New plant added at (" << sp.first << ',' << sp.second << ")\n";
                        // waterSource->capacity = waterSource->capacity - plt->waterConsumption();
                    }
                }
            }

            std::move(newPlants.begin(), newPlants.end(), std::back_inserter(plants));
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            const auto [x, y] = GetMousePosition();

            const float cx = camera.target.x, cy = camera.target.y;

            std::cout << "Mouse Pos is: " << static_cast<size_t>(x) << ',' << static_cast<size_t>(y) << '\n';
            // TODO: Make zoom work...
            const float cz = camera.zoom;
            const auto &elems = sas::findNearbyEntities<sas::Flower>(grid, static_cast<size_t>(x), static_cast<size_t>(y), 50);

            for (const auto &elem : elems)
            {
                std::print("Found element at pos {{{}, {}}}", elem->pos.x, elem->pos.y);
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            for (const auto &elem : grid)
            {
                std::print("Element in grid at pos{{{}, {}}}\n", elem.first.first, elem.first.second);
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