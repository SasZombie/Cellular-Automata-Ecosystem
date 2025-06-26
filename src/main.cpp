#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <deque>
#include <iostream>
#include <print>
#include <chrono>

// This is to ignore warnings from raylib that I cant do anything about
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wcast-qual"

#define RAYGUI_IMPLEMENTATION
#include "../Extern/raygui.h"

#pragma GCC diagnostic pop

#include "../include/Matrix.hpp"
#include "../include/Tile.hpp"
#include "../include/Grid.hpp"
#include "../include/Utils.hpp"
#include "../include/Common.hpp"
#include "../include/BoardOperations.hpp"
#include "../include/ConfigManager.hpp"
#include "../include/GameState.hpp"

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

static bool isValidInteger(const char *str, int &result)
{
    char *end;
    errno = 0;
    long val = std::strtol(str, &end, 10);

    if (errno != 0 || *end != '\0')
    {
        return false; // Conversion error or extra characters after number
    }

    if (val < INT_MIN || val > INT_MAX)
    {
        return false; // Out of int range
    }

    result = static_cast<int>(val);
    if (result > 500 || result < 0)
        return false;
    return true;
}

/// @brief
// board = background, no effect on anything --> Fastest
// enviroment = stuff that has collision such as water, needs a static grid --> Not as fast
// plant/rest = stuff that has collision but not necesarrly cellSize x cellSize, needs dynamic grid --> still fast but slower than the others
// This ^ also takes into account multiple entities on the same cell: 2 plants on the same cell, extra memory for this

int main()
{

    // size_t seed = sas::generateSeed();
    size_t seed = 30;
    // All sizes are in cells
    sas::ConfigManager::load("config.json");

    sas::Matrix<sas::Tile> board(WidthCells, HeightCells);

    std::vector<std::unique_ptr<sas::Enviroment>> enviroment;
    sas::StaticGrid enviromentGrid;

    std::vector<std::unique_ptr<sas::Plant>> plants;
    sas::DynamicGrid plantGrid;

    Camera2D camera;
    camera.target = {0.f, 0.f};
    camera.offset = {0.f, 0.f};
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    // For the ESC button, state machine
    const std::unordered_map<GameState, GameState> states = {
        {GameState::MENU, GameState::GAME},
        {GameState::GAME, GameState::MENU},
    };

    constexpr size_t FPS = 60;
    InitWindow(ScreenWidth, ScreenHeight, "Celular Automata Ecosystem");
    SetTargetFPS(FPS);

/////////////////////////////////
    sas::setUpBoardPerlin(board, seed);
    sas::setUpWaterNoise(enviroment, enviromentGrid, seed);
    sas::setUpInitialPlants(plants, plantGrid, enviromentGrid, enviroment);
////////////////////////////////

    // Tick = day
    // Season = ??
    float tickAcc = 0.f;
    float seasonAcc = 0.f;
    constexpr float intervalTick = 10.f / FPS;
    constexpr float intervalSeason = 16 * intervalTick;

    GameState currentState = GameState::GAME;

    char inputText[64] = "";

    bool textBoxActive = false;
    bool buttonClicked = false;
    bool showError = false;

    while (!WindowShouldClose())
    {

        switch (currentState)
        {
        case GameState::MENU:
        {
            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();

            Rectangle buttonBounds = {
                (screenWidth - 200) / 2.0f,
                (screenHeight - 40) / 2.0f - 60,
                200, 40};

            Rectangle textBoxBounds = {
                (screenWidth - 200) / 2.0f,
                (screenHeight - 30) / 2.0f,
                200, 30};

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                textBoxActive = CheckCollisionPointRec(GetMousePosition(), textBoxBounds);
            }

            BeginDrawing();
            ClearBackground(DARKGRAY);

            GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
            GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x5DADE2FF);
            GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x3498DBFF);
            GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x2E86C1FF);
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

            if (GuiButton(buttonBounds, "Start"))
            {
                buttonClicked = true;
            }

            GuiTextBox(textBoxBounds, inputText, 64, textBoxActive);

            if (buttonClicked)
            {
                int value = 0;
                buttonClicked = false;
                if (strlen(inputText) > 0)
                {
                    if (isValidInteger(inputText, value))
                    {
                        seed = static_cast<size_t>(value);
                        currentState = GameState::GAME;

                        sas::setUpBoardPerlin(board, seed);
                        sas::setUpWaterNoise(enviroment, enviromentGrid, seed);
                        sas::setUpInitialPlants(plants, plantGrid, enviromentGrid, enviroment);
                    }
                    else
                    {
                        showError = true;
                    }
                }
                else
                {
                    seed = sas::generateSeed();

                    currentState = GameState::GAME;

                    sas::setUpBoardPerlin(board, seed);
                    sas::setUpWaterNoise(enviroment, enviromentGrid, seed);
                    sas::setUpInitialPlants(plants, plantGrid, enviromentGrid, enviroment);
                }
            }

            if (showError)
            {
                int textWidth = MeasureText("Input is not number between 0 and 500", 20);
                DrawText("Input is not number 0 and 500",
                         (screenWidth - textWidth) / 2,
                         textBoxBounds.y + 50,
                         20, RAYWHITE);
            }

            EndDrawing();
        }
        break;
        case GameState::GAME:
            float dt = GetFrameTime();

            tickAcc = tickAcc + dt;

            if (tickAcc >= intervalTick)
            {
                tickAcc = 0;
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                sas::multiplyPlants(plantGrid, enviromentGrid, enviroment, plants);
                sas::spawnWeed(plantGrid, enviromentGrid, enviroment, plants);

                for (auto &plt : plants)
                {
                    plt->daysAlive++;
                }

                sas::killPlants(plantGrid, plants, enviroment);
            }

            if (IsKeyPressed(KEY_R))
            {
                sas::setUpBoardPerlin(board, seed);
            }

            if (IsKeyPressed(KEY_Q))
            {
                for (const auto &plt : plants)
                {
                    std::print("Position {}, {}, {}, {}\n", plt->pos.x, plt->pos.y, plt->pos.width, plt->pos.height);
                }
            }
            BeginDrawing();

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

            // DrawText(text.c_str(), 0, 0, 20, WHITE);

            EndDrawing();
        }
    }

    CloseWindow();
}