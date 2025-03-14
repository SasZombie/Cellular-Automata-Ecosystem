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


    //Storing the vectors in a Cashé, so we dont realocate each time 
    template <typename T>
    std::vector<T *> &getThreadLocalVector(size_t expectedSize = 32)
    {
        thread_local std::vector<T *> cache;
        cache.clear();
        if (cache.capacity() < expectedSize)
        {
            cache.reserve(expectedSize); // Avoid reallocations
        }
        return cache;
    }

    template <typename T, typename Func = auto (*)(const T &)->bool>
    std::vector<T *> findNearbyEntities(const Grid &grid, size_t x, size_t y, size_t range, Func &&func = [](const T &)
                                                                                            { return true; })
    {
        int startingPointX = x - range;
        int startingPointY = y - range;
        int stopPointX = x + range;
        int stopPointY = y + range;

        auto& results = getThreadLocalVector<T>();

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

        auto& results = getThreadLocalVector<Entity>();

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
