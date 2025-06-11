#include "../include/Grid.hpp"
#include <algorithm>

void sas::addToGrid(Grid &grid, Entity *entity)
{
    for (size_t dx = 0; dx < entity->pos.width; ++dx)
    {
        for (size_t dy = 0; dy < entity->pos.height; ++dy)
        {
            grid[{entity->pos.x + dx, entity->pos.y + dy}].push_back(entity);
        }
    }
}

void sas::removeFromGrid(Grid &grid, Entity *entity)
{
    auto &cell = grid[{entity->getPosition()}];
    cell.erase(std::remove(cell.begin(), cell.end(), entity), cell.end());
}
