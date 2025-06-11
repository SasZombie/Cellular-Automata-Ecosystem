#include "../include/DrawStrategy.hpp"

#include <raylib.h>


//I am not including the whole ahh header just for this
using std::size_t;


//Only god knows what all these comments are...
void sas::FlowerDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    const size_t x = pos.x, y = pos.y;

    size_t plantSize = 3;
    float fx = static_cast<float>(x), fy = static_cast<float>(y);


    // DrawTriangle({fx, fy}, {fx, fy - plantSize}, {fx - plantSize, fy}, PINK); // TopLeft
    // DrawTriangle({fx, fy}, {fx - plantSize, fy}, {fx, fy + plantSize}, PINK); // BottomLeft
    // DrawTriangle({fx, fy}, {fx, fy + plantSize}, {fx + plantSize, fy}, PINK); // BottomRight
    // DrawTriangle({fx, fy}, {fx + plantSize, fy}, {fx, fy - plantSize}, PINK); // TopRight

    // DrawCircle(0, 0, 20, RED);

    DrawTriangle({fx, fy + 2 * plantSize}, {fx, fy - 2 * plantSize}, {fx - 2 * plantSize, fy}, LIME); // LeftPart
    DrawTriangle({fx, fy + 2 * plantSize}, {fx + 2 * plantSize, fy}, {fx, fy - 2 * plantSize}, LIME); // Right
    DrawCircle(x - plantSize, y - plantSize, plantSize, PINK);
    DrawCircle(x - plantSize, y + plantSize, plantSize, PINK);
    DrawCircle(x + plantSize, y + plantSize, plantSize, PINK);
    DrawCircle(x + plantSize, y - plantSize, plantSize, PINK);
    DrawCircle(x, y, plantSize, RED);


    // DrawCircle(x, y, plantSize, PINK);
    // DrawLine(x - plantSize, y            , x + plantSize, y            , Color{150, 0, 100, 255});
    // DrawLine(x            , y - plantSize, x            , y + plantSize, Color{150, 0, 100, 255});
    // DrawCircle(x, y, plantSize / 3, RED);
    // size_t plantSize = 6;
    // size_t x = this->pos.first;
    // size_t y = this->pos.second;

    // DrawCircle(x, y, plantSize, PINK);
    // DrawLine(x - plantSize, y            , x + plantSize, y            , Color{150, 0, 100, 255});
    // DrawLine(x            , y - plantSize, x            , y + plantSize, Color{150, 0, 100, 255});
    // DrawCircle(x, y, plantSize >> 1, RED);
}

void sas::TreeDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    size_t plantSize = 10; // Impartire la 2
    const size_t x = pos.x, y = pos.y;

    DrawCircle(x, y, plantSize, Color{30, 215, 80, 255});
    DrawCircle(x                    , y + 2 * plantSize / 3 , 3 * plantSize >> 2, Color{0, 105, 0, 255});
    DrawCircle(x + (plantSize >> 1) , y                     , 3 * plantSize >> 2, Color{110, 180, 0, 255});
}

void sas::WeedDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    size_t x = pos.x, y = pos.y;

    size_t plantSize = 5; // Impartire la 2
    float fx = static_cast<float>(x), fy = static_cast<float>(y);

    DrawTriangle({fx + plantSize, fy + plantSize}, {fx + plantSize, fy - plantSize}, {fx, fy}, Color{80, 150, 80, 255});
}

void sas::PlaceholderDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    size_t x = pos.x, y = pos.y;

    size_t plantSize = 5; // Impartire la 2
    float fx = static_cast<float>(x), fy = static_cast<float>(y);

    DrawCircle(x, y, plantSize, RED);

}

void sas::WaterDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    DrawRectangle(pos.x, pos.y, pos.width, pos.height, BLUE);
    DrawRectangleLines(pos.x, pos.y, pos.width, pos.height, BLACK);
}

void sas::DesertDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    DrawRectangle(pos.x, pos.y, pos.width, pos.height, {170, 170, 0, 255});
    DrawRectangleLines(pos.x, pos.y, pos.width, pos.height, RED);

}

void sas::SnowDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    DrawRectangle(pos.x, pos.y, pos.width, pos.height, {200, 200, 200, 255});
    DrawRectangleLines(pos.x, pos.y, pos.width, pos.height, RED);
}

void sas::GrassDrawStrategy::draw(const sas::Position &pos) const noexcept
{
    DrawRectangle(pos.x, pos.y, pos.width, pos.height, {0, 128, 0, 255});
    DrawRectangleLines(pos.x, pos.y, pos.width, pos.height, RED);
}
