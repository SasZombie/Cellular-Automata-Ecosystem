#pragma once

#include "Entity.hpp"
#include <unordered_map>
#include <functional>
#include <vector>
#include <print>
#include <iostream>
#include <unordered_set>

namespace sas
{

    constexpr size_t spatialCellSize = 50; // or cellSize

    struct PairHash
    {
        std::size_t operator()(const std::pair<size_t, size_t> &p) const noexcept
        {
            return std::hash<size_t>()(p.first) ^ (std::hash<size_t>()(p.second) << 1);
        }
    };

    using GridPos = std::pair<size_t, size_t>;

    //Choices, choices...
    //Fastest = grid has indicies in an vector 
    //We have to pass the gird and the vector everywhere
    //OR
    //Not as fast = grid has a pointer to elements in vector
    //Combination of shared_ptr + weak_ptr
    //Or just a raw pointer

    //We could this:
    /*
        struct gridVector{
            DynamicGrid d;
            std::vector<..> elems;
        };
    */
    
    using StaticGrid = std::unordered_map<GridPos, size_t, PairHash>;

    using DynamicGrid = std::unordered_map<GridPos, std::vector<Entity *>, PairHash>;

    void addToGrid(DynamicGrid &grid, Entity *entity, size_t index) noexcept;
    void removeFromGrid(DynamicGrid &grid, Entity *entity) noexcept;

    // Entity <-> Enviroment
    bool collides(const Position &entity, const StaticGrid &grid);
    // Entity <-> Entity!
    template <typename T>
    bool collides(const Position& entity, const std::vector<std::unique_ptr<T>>& entities, const DynamicGrid &grid) noexcept
    {
        int left = entity.x / spatialCellSize;
        int right = (entity.x + entity.width) / spatialCellSize;
        int top = entity.y / spatialCellSize;
        int bottom = (entity.y + entity.height) / spatialCellSize;

        for (int gx = left; gx <= right; ++gx)
        {
            for (int gy = top; gy <= bottom; ++gy)
            {
                auto it = grid.find({gx, gy});
                if (it == grid.end())
                    continue;

                for (const Entity* other : it->second)
                {
                    // Skip self
                    if (other->pos == entity)
                        continue; 


                    bool overlap =
                        entity.x < other->pos.x + other->pos.width &&
                        entity.x + entity.width > other->pos.x &&
                        entity.y < other->pos.y + other->pos.height &&
                        entity.y + entity.height > other->pos.y;

                    if (overlap)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool isNear(const Entity *entity, const StaticGrid &grid, int maxRange = 0) noexcept;
    bool isNear(const Entity *entity, const DynamicGrid &grid, int maxRange = 0) noexcept;

} // namespace sas
