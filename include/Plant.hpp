#pragma once
#include <cstddef>
#include "Entity.hpp"
#include <vector>

namespace sas
{
    // This is bad if we want to have more than 10 plants btw
    enum struct PlantType
    {
        TREE,
        FLOWER,
        WEED
    };

    class Plant : public Entity
    {
    public:
        size_t nrOfSeeds;
        size_t rangeSpreadingSeeds;
        size_t rangeWater;
        size_t size;
        size_t daysAlive = 0;

        Plant(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat,
              size_t nrOfSeeds, size_t rangeSpreadingSeeds, size_t rangeWater, size_t size) noexcept;

        std::vector<std::pair<size_t, size_t>> reproduce() const noexcept;
        virtual bool willWither() const noexcept = 0;
        virtual size_t waterConsumption() const noexcept = 0;
        virtual size_t range() const noexcept = 0;

        virtual PlantType getPlantType() const noexcept = 0;

        virtual ~Plant() = default;
    };

    class Tree : public Plant
    {
    public:
        Tree(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat = nullptr) noexcept;

        PlantType getPlantType() const noexcept;

        size_t waterConsumption() const noexcept;
        bool willWither() const noexcept;
        size_t range() const noexcept;
    };

    class Flower : public Plant
    {
    public:
        Flower(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat = nullptr) noexcept;
        PlantType getPlantType() const noexcept;

        size_t range() const noexcept;
        bool willWither() const noexcept;
        size_t waterConsumption() const noexcept;
    };

    class Weed : public Plant
    {
    public:
        Weed(size_t x, size_t y, std::shared_ptr<DrawStrategy> strat = nullptr) noexcept;

        PlantType getPlantType() const noexcept;

        size_t range() const noexcept;
        bool willWither() const noexcept;
        size_t waterConsumption() const noexcept;
    };
}