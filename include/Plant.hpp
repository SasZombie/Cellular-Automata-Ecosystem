#pragma once
#include <cstddef>
#include "Entity.hpp"
#include <vector>

namespace sas
{
    //This is bad if we want to have more than 10 plants btw
    enum struct PlatType
    {
        TREE,
        FLOWER,
        WEED
    };

    class Plant : public Entity
    {
    public:
        size_t size = 20;
        size_t rangeWater, rangeSpreadingSeeds = 100;
        size_t nrOfSeeds = 1;

        std::vector<std::pair<size_t, size_t>> reproduce() const noexcept;
        virtual size_t waterConsumption() const noexcept = 0; 
        virtual size_t range() const noexcept = 0; 
        virtual void draw() const noexcept = 0;

        virtual ~Plant() = default;
    };

    class Tree: public Plant
    {
    public:

        size_t waterConsumption() const noexcept;

        void draw() const noexcept;
        std::pair<size_t, size_t> getPosition() const;

        size_t range() const noexcept;
    };

    class Flower: public Plant
    {
    public:

        size_t range() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw() const noexcept;
    };

    class Weed: public Plant
    {
    public:
        size_t range() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw() const noexcept;
    };
}