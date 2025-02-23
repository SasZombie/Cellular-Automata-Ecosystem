#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include "Shape.hpp"
#include "../include/matrix.hpp"
#include "../include/Tile.hpp"
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

void DrawBoard(sas::Tile board[], Vector2 offset);
void SetUpBoard(sas::Tile board[]);

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
            DrawRectangle(j * 1.1f * cellSize + offset.x, i * 1.1f * cellSize + offset.y, cellSize, cellSize, color);

            std::visit([](const auto &ptr)
                       {
                if constexpr (!std::is_same_v<std::monostate, std::decay_t<decltype(ptr)>>)
                {
                    if constexpr (std::is_base_of_v<sas::Plant, std::decay_t<decltype(*ptr)>>)
                    {
                        ptr->draw(30, 30);
                    }
                } }, board(i, j).occupant);
        }
    }
}



//This will be moved to UTILS
void generateNumbers(sas::Matrix<sas::Tile> &board)
{

}

// template <typename Func>
// void check(const sas::Matrix<sas::Tile> &board, const sas::Enviroment &elem, Func&& condition)
// {
//     for (const auto &each : board)
//     {
//         std::visit([&](const auto &ptr)
//                    {
//                 if constexpr (!std::is_same_v<std::monostate, std::decay_t<decltype(ptr)>>)
//                 {
//                     if constexpr (std::is_same_v<std::decay_t<decltype(elem)>, std::decay_t<decltype(*ptr)>> ||
//                         std::is_same_v<std::decay_t<decltype(elem)>, std::decay_t<decltype(*ptr)>>)
//                     {
//                         if(condition(*ptr))
//                             std::cout << "Found\n";
//                     }
//                 } }, each.occupant);
//     }
// }

int main()
{

    // // De citit
    // sas::Tile t;
    // t.occupant = std::make_unique<sas::Flower>();

    // std::visit([](const auto &ptr)
    //            {
    //                 if constexpr (std::is_base_of_v<std::monostate, std::decay_t<decltype(*ptr)>>)
    //                 {
    //                     gol;
    //                 }
    //                 else
    //                 {

    //                 }

    //                if constexpr (std::is_base_of_v<sas::Plant, std::decay_t<decltype(*ptr)>>)
    //                {
    //                    ptr->draw();
    //                } },
    //            t.occupant);

    // // De
    sas::Matrix<sas::Tile> board(boardHeight, boardWidth);

    Vector2 boardOffset{0.f, 0.f};

    board(0, 0).addPlant(std::make_unique<sas::Flower>());
    board(10, 10).addPlant(std::make_unique<sas::Flower>());

    board(12, 12).addEnviroment(std::make_unique<sas::Water>());

    // check(board, sas::Water(), [&](const sas::Enviroment& env)
    // {
    //     return true;
    // });

    // How to see closest! ;D
    

    SetUpBoard(board);

    Camera2D camera;
    camera.target = {300.f, 300.f};
    camera.offset = {ScreenWidth / 2.0f, ScreenHeight / 2.0f};
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    InitWindow(ScreenWidth, ScreenHeight, "Celular Automata Ecosystem");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_D))
            camera.target.x += 2;
        else if (IsKeyDown(KEY_A))
            camera.target.x -= 2;
        if (IsKeyDown(KEY_W))
            camera.target.y -= 2;
        else if (IsKeyDown(KEY_S))
            camera.target.y += 2;

        // camera.target = boardOffset;

        // if (IsKeyDown(KEY_A)) camera.rotation--;
        // else if (IsKeyDown(KEY_S)) camera.rotation++;
        // if (camera.rotation > 40) camera.rotation = 40;
        // else if (camera.rotation < -40) camera.rotation = -40;

        // Camera zoom controls
        if (IsKeyDown(KEY_MINUS))
            camera.zoom += 0.005f;
        else if (IsKeyDown(KEY_EQUAL))
            camera.zoom -= 0.005f;

        camera.zoom += ((float)GetMouseWheelMove() * -0.05f);

        // if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        // else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

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
        DrawText("Plus si minus pt zoom\nWASD pentru movement", -20, -60, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}