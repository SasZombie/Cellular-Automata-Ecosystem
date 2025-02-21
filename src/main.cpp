#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include "Shape.hpp"
#include "Tile.hpp"


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

void DrawBoard(Tiles board[], Vector2 offset);
void SetUpBoard(Tiles board[]);

void SetUpBoard(Tiles board[]) {
    for(size_t i = 0; i < boardSize / boardWidth; ++i)
        for(size_t j = 0; j < boardWidth; ++j) {
            if     (j <= 1.f * boardWidth / 6 || j > 5.f * boardWidth / 6)
                board[i * boardWidth + j] = Snow;
            else if(j <= 5.f * boardWidth / 12 || j > 7.f * boardWidth / 12)
                board[i * boardWidth + j] = Grass;
            else
                board[i * boardWidth + j] = Sand;
        }
}

void DrawBoard(Tiles board[], Vector2 offset) {
    for(size_t i = 0; i < boardSize / boardWidth; ++i)
        for(size_t j = 0; j < boardWidth; ++j) {
            Color color;
            switch(board[i * boardWidth + j]) {
                case Tiles::Snow:  color = Color{ 200, 200, 200, 255 }; break;
                case Tiles::Grass: color = Color{  0 , 128,  0 , 255 }; break;
                case Tiles::Sand:  color = Color{ 170, 170,  0 , 255 }; break;
            }
            DrawRectangle(j * 1.1f * cellSize + offset.x, i * 1.1f * cellSize + offset.y, cellSize, cellSize, color);
        }
}

int main()
{
    Tiles board[boardWidth * boardHeight];
    SetUpBoard(board);

    Vector2 boardOffset{ 0.f, 0.f };

    Camera2D camera = { 0 };
    camera.target = (Vector2) { 300.f, 300.f };
    camera.offset = (Vector2) { ScreenWidth/2.0f, ScreenHeight/2.0f };
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    InitWindow(ScreenWidth, ScreenHeight, "Langhton's Ant");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_D)) boardOffset.x += 2;
        else if (IsKeyDown(KEY_A)) boardOffset.x -= 2;
        if (IsKeyDown(KEY_W)) boardOffset.y -= 2;
        else if (IsKeyDown(KEY_S)) boardOffset.y += 2;

        // camera.target = boardOffset;

        // if (IsKeyDown(KEY_A)) camera.rotation--;
        // else if (IsKeyDown(KEY_S)) camera.rotation++;
        // if (camera.rotation > 40) camera.rotation = 40;
        // else if (camera.rotation < -40) camera.rotation = -40;

        // Camera zoom controls
        if (IsKeyDown(KEY_MINUS)) camera.zoom += 0.005f;
        else if (IsKeyDown(KEY_EQUAL)) camera.zoom -= 0.005f;

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