#pragma once
#include <cstddef>
#include "Entity.hpp"
#include <vector>

namespace sas
{
    //This is bad if we want to have more than 10 plants btw
    enum struct PlantType
    {
        TREE,
        FLOWER,
        WEED
    };

    class Plant : public Entity
    {
    public:
        size_t size = 20;
        size_t rangeWater = 100, rangeSpreadingSeeds = 50;
        size_t nrOfSeeds = 1;
        size_t daysAlive = 0;

        std::vector<std::pair<size_t, size_t>> reproduce() const noexcept;
        virtual bool willWither() const noexcept = 0;
        virtual size_t waterConsumption() const noexcept = 0; 
        virtual size_t range() const noexcept = 0; 

        virtual PlantType getPlantType() const noexcept = 0;

        virtual ~Plant() = default;
    };

    class Tree: public Plant
    {
    public:
        
        PlantType getPlantType() const noexcept;

        size_t waterConsumption() const noexcept;
        bool willWither() const noexcept;
        size_t range() const noexcept;
    };

    class Flower: public Plant
    {
    public:

        PlantType getPlantType() const noexcept;


        size_t range() const noexcept;
        bool willWither() const noexcept;
        size_t waterConsumption() const noexcept;

    };

    class Weed: public Plant
    {
    public:
        PlantType getPlantType() const noexcept;

        size_t range() const noexcept;
        bool willWither() const noexcept;
        size_t waterConsumption() const noexcept;

    };
}