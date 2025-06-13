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

    using StaticGrid = std::unordered_set<std::pair<size_t, size_t>, PairHash>;

    using DynamicGrid = std::unordered_map<GridPos, std::vector<size_t>, PairHash>;

    void addToGrid(DynamicGrid &grid, const Entity *entity, size_t index) noexcept;
    void removeFromGrid(StaticGrid &grid, Entity *entity);

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

                for (const size_t other : it->second)
                {
                    // Skip self
                    if (entities[other]->pos == entity)
                        continue; 

                    const Entity *f2 = entities[other].get();

                    bool overlap =
                        entity.x < f2->pos.x + f2->pos.width &&
                        entity.x + entity.width > f2->pos.x &&
                        entity.y < f2->pos.y + f2->pos.height &&
                        entity.y + entity.height > f2->pos.y;

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
