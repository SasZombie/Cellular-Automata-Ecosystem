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
    plants.push_back(sas::plantFactory(grid, 10, 10, sas::PlatType::FLOWER));
    plants.push_back(sas::plantFactory(grid, 0, 10, sas::PlatType::WEED));


    //EXAMPLES =========================================================================

    const auto& elems = sas::findNearbyEntities<sas::Entity>(grid, 0, 0, 10);

    for(const auto& elem : elems)
    {
        std::cout <<elem->pos.first << ' ' << elem->pos.second << '\n' ;
    }

    const auto &elems2 = sas::findNearbyEntities<sas::Flower>(grid, 0, 0, 10);

    for(const auto& elem : elems2)    
    {
        std::cout <<elem->pos.first << ' ' << elem->pos.second << '\n' ;
    }


    const auto &elems3 = sas::findNearbyEntities<sas::Entity>(grid, 0, 0, 10, [](const sas::Entity& ent)
    {
        if(ent.pos.first == 20)
            return true;
        return false;
    });

    for(const auto& elem : elems3)    
    {
        std::cout <<elem->pos.first << ' ' << elem->pos.second << '\n' ;
    }
    std::cout << "Nothing as expected\n";

    //EXAMPLES =========================================================================

    
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

        if(timeAcc >= interval)
        {
            timeAcc = 0;

            const auto [x, y] = sas::generateNextPos();
            // Cate plante adugam, unde le 
            for(const auto& plt : plants)
            {
                const auto& ghinde = plt->reproduce();

                for(const auto& elem : ghinde)
                {
                    //Elem = (x, y);
                    // TODO: remove Animals or whatever
                    const auto& neighboutrs = sas::findNearbyEntities<sas::Entity>(grid, elem.first, elem.second, plt->rangeSpreadingSeeds);

                    // if(sas::checkBoundaries(elem, plt->size(), ))
                }
            }

            plants.push_back(sas::plantFactory(grid, (x + 0.5f) * cellSize, (y + 0.5f) * cellSize, sas::PlatType::FLOWER));
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

        for(const auto& plt : plants)
        {
            plt->draw();
        }

        DrawText(text.c_str(), -20, -60, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}