#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include "Shape.hpp"
#include "../include/matrix.hpp"
#include "../include/Tile.hpp"
#include "../include/Grid.hpp"
#include "../include/Utils.hpp"

constexpr size_t ScreenWidth = 800;
constexpr size_t ScreenHeight = 800;
constexpr float cellSize = 20;
// constexpr float boardPartSize = 8;
// constexpr size_t boardSideLength = boardPartSize * cellSize;
// constexpr size_t boardPartsHorizontally = ScreenWidth / boardSideLength;
// constexpr size_t boardPartsVertically = ScreenHeight / boardSideLength;

constexpr size_t boardWidth = 30;
constexpr size_t boardHeight = 20;
constexpr size_t boardSize = boardWidth * boardHeight;

void DrawBoard(const sas::Matrix<sas::Tile> &board, Vector2 offset);
void SetUpBoard(sas::Matrix<sas::Tile> &board);

void SetUpBoard(sas::Matrix<sas::Tile> &board)
{
    for (size_t i = 0; i < boardHeight; ++i)
    {
        for (size_t j = 0; j < boardWidth; ++j)
        {
            if (j <= 1.f * boardWidth / 6 || j > 5.f * boardWidth / 6)
            {
                board(i, j).type = sas::TileType::SNOW;
            }
            else if (j <= 5.f * boardWidth / 12 || j > 7.f * boardWidth / 12)
            {
                board(i, j).type = sas::TileType::GRASS;
            }
            else
            {
                board(i, j).type = sas::TileType::DESERT;
            }
        }
    }
}

void DrawBoard(const sas::Matrix<sas::Tile> &board, Vector2 offset)
{
    for (size_t i = 0; i < boardHeight; ++i)
    {

        for (size_t j = 0; j < boardWidth; ++j)
        {
            Color color;
            switch (board(i, j).type)
            {
            case sas::TileType::SNOW:
                color = Color{200, 200, 200, 255};
                break;
            case sas::TileType::GRASS:
                color = Color{0, 128, 0, 255};
                break;
            case sas::TileType::DESERT:
                color = Color{170, 170, 0, 255};
                break;
            }
            DrawRectangle(j * cellSize + offset.x, i * cellSize + offset.y, cellSize, cellSize, color);
        }
    }
}

int main()
{
    sas::Matrix<sas::Tile> board(boardHeight, boardWidth);

    size_t seed = sas::generateSeed();

    sas::Grid grid;
    std::vector<std::unique_ptr<sas::Plant>> plants;

    plants.push_back(sas::plantFactory(grid, 0, 0, sas::PlatType::FLOWER));

    Vector2 boardOffset{0.f, 0.f};

    SetUpBoard(board);

    Camera2D camera;
    camera.target = {300.f, 300.f};
    camera.offset = {ScreenWidth / 2.0f, ScreenHeight / 2.0f};
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    constexpr size_t FPS = 60;
    InitWindow(ScreenWidth, ScreenHeight, "Celular Automata Ecosystem");
    SetTargetFPS(FPS);

    float timeAcc = 0.f;
    constexpr float interval = 10.f / FPS;
    std::string text = "Plus si minus pt zoom\nWASD pentru movement Seed = " + std::to_string(seed);

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();

        timeAcc = timeAcc + dt;

        if (timeAcc >= interval)
        {
            timeAcc = 0;
        }

        // Cate plante adugam, unde le

        if (IsKeyPressed(KEY_SPACE))
        {
            std::vector<std::unique_ptr<sas::Plant>> newPlants;

            for (const auto &plt : plants)
            {
                const auto &spawnPoints = plt->reproduce();
                for (const auto &sp : spawnPoints)
                {
                    // Elem = (x, y);
                    //All are 20 isntead of 40. TO CHEK!
                    if (sas::checkBoundaries(sp, plt->pos, plt->size, plt->size))
                    {

                        bool canPlant = true;
                        //  TODO: remove Animals or whatever
                        // So far this finds only plants!!!
                        const auto &neighbours = sas::findNearbyEntities<sas::Enviroment>(grid, sp.first, sp.second, plt->rangeSpreadingSeeds);
                        for (const auto &neighbour : neighbours)
                        {
                            // This should be changed if we want to also track something else
                            // if (!sas::checkBoundaries(neighbour->pos, plt->pos, plt->size + plt->size))
                            if (!sas::checkBoundaries(sp, neighbour->pos, plt->size, 17)) // am zis 17 ca 10sqrt(2) pt diagonala unui bloc de apa
                            {                                                             // not proud of this one tbh
                                canPlant = false;
                                break;
                            }
                        }

                        const auto &neighbours = sas::findNearbyEntities<sas::Plant>(grid, sp.first, sp.second, plt->rangeSpreadingSeeds);
                        for (const sas::Plant &neighbour : neighbours)
                        {
                            // This should be changed if we want to also track something else
                            // if (!sas::checkBoundaries(neighbour->pos, plt->pos, plt->size + plt->size))
                            if (!sas::checkBoundaries(sp, neighbour->pos, plt->size, neighbour->size);
                            {
                                canPlant = false;
                                break;
                            }
                        }

                        if (canPlant)
                        {
                            std::cout << "Added at: " << sp.first << ' ' << sp.second << " from initial: " << plt->pos.first << ' ' << plt->pos.second << '\n';
                            newPlants.push_back(sas::plantFactory(grid, sp.first, sp.second, sas::PlatType::FLOWER));
                        }
                    }
                }
            }

            std::move(newPlants.begin(), newPlants.end(), std::back_inserter(plants));
        }

        if(IsKeyPressed(KEY_R))
        {
            plants.clear();
            grid.clear();
            plants.push_back(sas::plantFactory(grid, 0, 0, sas::PlatType::FLOWER));
        }

        if (IsKeyDown(KEY_D))
            camera.target.x += 2;
        else if (IsKeyDown(KEY_A))
            camera.target.x -= 2;
        if (IsKeyDown(KEY_W))
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

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(Color{18, 18, 18, 255});
        DrawBoard(board, boardOffset);

        for (const auto &plt : plants)
        {
            plt->draw();
        }

        DrawText(text.c_str(), -20, -60, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}