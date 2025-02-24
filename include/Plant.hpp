#pragma once
#include <cstddef>
#include "Entity.hpp"

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
        virtual bool reproduce() const noexcept = 0;
        virtual size_t waterConsumption() const noexcept = 0; 
        virtual size_t range() const noexcept = 0; 
        virtual void draw() const noexcept = 0;

        virtual ~Plant() = default;
    };

    class Tree: public Plant
    {
    public:
        bool reproduce() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw() const noexcept;
        std::pair<size_t, size_t> getPosition() const;

        size_t range() const noexcept;
    };

    class Flower: public Plant
    {
    public:
        bool reproduce() const noexcept;
        size_t range() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw() const noexcept;
    };

    class Weed: public Plant
    {
    public:
        bool reproduce() const noexcept;
        size_t range() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw() const noexcept;
    };
}