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
#pragma GCC diagnostic ignored "-Wstringop-overflow"


#define RAYGUI_IMPLEMENTATION
#include "../Extern/raygui.h"
#include "raylib.h"
#include "raymath.h"

#pragma GCC diagnostic pop

#include "../include/Game.hpp"
#include "../include/Matrix.hpp"
#include "../include/Tile.hpp"
#include "../include/Grid.hpp"
#include "../include/Utils.hpp"
#include "../include/Common.hpp"
#include "../include/BoardOperations.hpp"
#include "../include/ConfigManager.hpp"

char inputText[64] = "";

bool textBoxActive = false;
bool buttonClicked = false;
bool showError = false;

Rectangle buttonBounds = {
    (ScreenWidth - 200) / 2.0f,
    (ScreenHeight - 40) / 2.0f - 60,
    200, 40};

Rectangle textBoxBounds = {
    (ScreenWidth - 200) / 2.0f,
    (ScreenHeight - 30) / 2.0f,
    200, 30};

static void showUi(sas::Game& game)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        textBoxActive = CheckCollisionPointRec(GetMousePosition(), textBoxBounds);
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x5DADE2FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x2E86C1FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

    if (GuiButton(buttonBounds, "Start"))
    {
        buttonClicked = true;
    }

    GuiTextBox(textBoxBounds, inputText, 64, textBoxActive);

    if (buttonClicked)
    {
        size_t value = 0;
        buttonClicked = false;
        if (strlen(inputText) > 0)
        {
            if (sas::isValidInteger(inputText, value))
            {
                game.currentState = GameState::GAME;
                game.setUp(value);
            }
            else
            {
                showError = true;
            }
        }
        else
        {
            game.currentState = GameState::GAME;
        }
    }

    if (showError)
    {
        int textWidth = MeasureText("Input is not number between 0 and 500", 20);
        DrawText("Input is not number between 0 and 500",
                 (ScreenWidth - textWidth) / 2,
                 textBoxBounds.y + 50,
                 20, RAYWHITE);
    }

    EndDrawing();
}


int main()
{
    size_t seed = sas::generateSeed();
    
    sas::ConfigManager::load("config.json");
    sas::Game game(seed);

    Camera2D camera;
    camera.target = {0.f, 0.f};
    camera.offset = {0.f, 0.f};
    camera.zoom = -1.0f;
    camera.rotation = 180.0f;

    constexpr size_t FPS = 60;
    InitWindow(ScreenWidth, ScreenHeight, "Celular Automata Ecosystem");
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {

        switch (game.currentState)
        {
        case GameState::MENU:
        {
            showUi(game);
        }
        break;
        case GameState::GAME:

            if (IsKeyPressed(KEY_SPACE))
            {
                game.passTime();
            }

            BeginDrawing();
            ClearBackground(GRAY);
            
            BeginMode2D(camera);

            sas::handleCameraControlls(camera);

            //I like doing the drawing in here
            //Rather than abstracting this to a 
            //Draw all function
            for (const auto &tile : game.boardRef())
            {
                tile.draw();
            }

            for (const auto &env : game.enviromentRef())
            {
                env->draw();
            }

            for (const auto &plt : game.plantsRef())
            {
                plt->draw();
            }

            EndDrawing();
        }
    }
    CloseWindow();
}