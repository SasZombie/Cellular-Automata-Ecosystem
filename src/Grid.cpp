#include "../include/Grid.hpp"
#include "../include/Common.hpp"

#include <algorithm>
#include <assert.h>
#include <iostream>

void sas::addToGrid(DynamicGrid &grid, const Entity *entity, size_t index) noexcept
{
    int left = entity->pos.x / spatialCellSize;
    int right = (entity->pos.x + entity->pos.width) / spatialCellSize;
    int top = entity->pos.y / spatialCellSize;
    int bottom = (entity->pos.y + entity->pos.height) / spatialCellSize;

    for (int gx = left; gx <= right; ++gx)
    {
        for (int gy = top; gy <= bottom; ++gy)
        {
            grid[{static_cast<size_t>(gx), static_cast<size_t>(gy)}].push_back(index);
        }
    }
}

void sas::removeFromGrid(StaticGrid &grid, Entity *entity)
{
    assert(false && "TODO!");
    // auto &cell = grid[{entity->getPosition()}];
    // cell.erase(std::remove(cell.begin(), cell.end(), entity), cell.end());
}

bool sas::collides(const Position &entity, const StaticGrid &grid)
{
    int left = entity.x / cellSize;
    int right = (entity.x + entity.width - 1) / cellSize;
    int top = entity.y / cellSize;
    int bottom = (entity.y + entity.height - 1) / cellSize;

    for (int gx = left; gx <= right; ++gx)
    {
        for (int gy = top; gy <= bottom; ++gy)
        {
            if (grid.contains({gx, gy}))
            {
                // std::cout << "Colides with water!\n";
                return true;
            }
        }
    }
    return false;
}


bool sas::isNear(const Entity* entity, const StaticGrid &grid, int maxRange) noexcept
{
    int centerX = (entity->pos.x + entity->pos.width / 2) / cellSize;
    int centerY = (entity->pos.y + entity->pos.height / 2) / cellSize;

    for (int dx = -maxRange; dx <= maxRange; ++dx)
    {
        for (int dy = -maxRange; dy <= maxRange; ++dy)
        {
            if (abs(dx) + abs(dy) > maxRange)
                continue;

            if (grid.contains({centerX + dx, centerY + dy}))
            {
                return true;
            }
        }
    }
    return false;
}

bool sas::isNear(const Entity* entity, const DynamicGrid &grid, int maxRange) noexcept
{
    assert(false && "TODO: is near with Dynamic!");
    return false;
    // int centerX = (entity->pos.x + entity->pos.width / 2) / cellSize;
    // int centerY = (entity->pos.y + entity->pos.height / 2) / cellSize;

    // for (int dx = -maxRange; dx <= maxRange; ++dx)
    // {
    //     for (int dy = -maxRange; dy <= maxRange; ++dy)
    //     {
    //         if (abs(dx) + abs(dy) > maxRange)
    //             continue;

    //         if (grid.contains({centerX + dx, centerY + dy}))
    //         {
    //             return true;
    //         }
    //     }
    // }

    // std::cout << "Is not near water\n";
    // return false;
}