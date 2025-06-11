#pragma once

#include "Entity.hpp"
#include <unordered_map>
#include <functional>
#include <vector>
#include <print>

namespace sas
{

    struct PairHash
    {
        std::size_t operator()(const std::pair<size_t, size_t> &p) const noexcept
        {
            return std::hash<size_t>()(p.first) ^ (std::hash<size_t>()(p.second) << 1);
        }
    };

    using Grid = std::unordered_map<std::pair<size_t, size_t>, std::vector<Entity *>, PairHash>;
    void addToGrid(Grid &grid, Entity *entity);
    void removeFromGrid(Grid &grid, Entity *entity);
    // std::vector<sas::Entity *> findNearbyEntities(const Grid& grid, size_t x, size_t y);

    // Storing the vectors in a Cashé, so we dont realocate each time
    template <typename T>
    std::vector<T *> &getThreadLocalVector(size_t expectedSize = 32)
    {
        thread_local std::vector<T *> cache;
        cache.clear();
        if (cache.capacity() < expectedSize)
        {
            cache.reserve(expectedSize);
        }
        return cache;
    }

    // MUST MAKE A WAY TO ADD WIDTH OR HEIGHT TO THIS
    template <typename T, typename Func = auto (*)(const T &)->bool>
    std::vector<T *> findNearbyEntities(const Grid &grid, size_t x, size_t y, size_t range, Func &&func = [](const T &)
                                                                                            { return true; })
    {
        int searchMinX = static_cast<int>(x) - static_cast<int>(range);
        int searchMaxX = static_cast<int>(x) + static_cast<int>(range);
        int searchMinY = static_cast<int>(y) - static_cast<int>(range);
        int searchMaxY = static_cast<int>(y) + static_cast<int>(range);

        auto &results = getThreadLocalVector<T>();

        for (int dx = searchMinX; dx <= searchMaxX; ++dx)
        {
            for (int dy = searchMinY; dy <= searchMaxY; ++dy)
            {
                auto it = grid.find({dx, dy});
                if (it != grid.end())
                {
                    for (Entity *entity : it->second)
                    {
                        if (T *specificEntity = dynamic_cast<T *>(entity))
                        {
                            size_t entMinX = specificEntity->pos.x;
                            size_t entMaxX = specificEntity->pos.x + specificEntity->pos.width;
                            size_t entMinY = specificEntity->pos.y;
                            size_t entMaxY = specificEntity->pos.y + specificEntity->pos.height;

                            // Check for bounding box intersection
                            bool intersects = entMaxX >= static_cast<size_t>(searchMinX) &&
                                              entMinX <= static_cast<size_t>(searchMaxX) &&
                                              entMaxY >= static_cast<size_t>(searchMinY) &&
                                              entMinY <= static_cast<size_t>(searchMaxY);

                            if (intersects && func(*specificEntity))
                            {
                                results.push_back(specificEntity);
                            }
                        }
                    }
                }
            }
        }

        return results;
    }

    template <typename Func = auto (*)(const sas::Entity &)->bool>
    std::vector<sas::Entity *> findNearbyEntities(const Grid &grid, size_t x, size_t y, size_t range, Func &&func)
    {

        int startingPointX = x - range;
        int startingPointY = y + range;
        int stopPointX = x + range;
        int stopPointY = y + range;

        auto &results = getThreadLocalVector<Entity>();

        for (int dx = startingPointX; dx <= stopPointX; ++dx)
        {
            for (int dy = startingPointY; dy <= stopPointY; ++dy)
            {
                auto it = grid.find({dx, dy});
                if (it != grid.end())
                {
                    for (Entity *entity : it->second)
                    {
                        if (func(entity))
                        {
                            results.push_back(entity);
                        }
                    }
                }
            }
        }
        return results;
    }

    // Returns nullptr if not found
    template <typename T, typename Func = auto (*)(const T &)->bool>
    T *findNearestEntity(const Grid &grid, size_t x, size_t y, size_t range, Func &&func = [](const T &)
                                                                             { return true; })
    {
        int startingPointX = x - range;
        int startingPointY = y - range;
        int stopPointX = x + range;
        int stopPointY = y + range;

        for (int dx = startingPointX; dx <= stopPointX; ++dx)
        {
            for (int dy = startingPointY; dy <= stopPointY; ++dy)
            {
                auto it = grid.find({dx, dy});
                if (it != grid.end())
                {
                    for (Entity *entity : it->second)
                    {
                        if (T *specificEntity = dynamic_cast<T *>(entity))
                        {
                            if (func(*specificEntity))
                            {
                                return specificEntity;
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    // Returns nullptr if not found
    template <typename Func = auto (*)(const sas::Entity &)->bool>
    sas::Entity *findNearestEntity(const Grid &grid, size_t x, size_t y, size_t range, Func &&func)
    {

        int startingPointX = x - range;
        int startingPointY = y + range;
        int stopPointX = x + range;
        int stopPointY = y + range;

        for (int dx = startingPointX; dx <= stopPointX; ++dx)
        {
            for (int dy = startingPointY; dy <= stopPointY; ++dy)
            {
                auto it = grid.find({dx, dy});
                if (it != grid.end())
                {
                    for (Entity *entity : it->second)
                    {
                        if (func(entity))
                        {
                            return entity;
                        }
                    }
                }
            }
        }
        return nullptr;
    }

} // namespace sas
