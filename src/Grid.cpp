#include "../include/Grid.hpp"
#include <algorithm>


void sas::addToGrid(Grid &grid, Entity *entity)
{
    grid[{entity->getPosition()}].push_back(entity);
}

void sas::removeFromGrid(Grid &grid, Entity *entity)
{
    auto &cell = grid[{entity->getPosition()}];
    cell.erase(std::remove(cell.begin(), cell.end(), entity), cell.end());
}
