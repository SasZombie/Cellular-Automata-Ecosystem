#pragma once
#include <cstddef>

namespace sas
{
    class Plant
    {
    public:
        virtual bool reproduce() const noexcept = 0;
        virtual size_t waterConsumption() const noexcept = 0; 
        virtual void draw(int x, int y) const noexcept = 0;
        virtual ~Plant() = default;
    };

    class Tree: public Plant
    {
    public:
        bool reproduce() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw(int x, int y) const noexcept;
    };

    class Flower: public Plant
    {
    public:
        bool reproduce() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw(int x, int y) const noexcept;
    };

    class Weed: public Plant
    {
    public:
        bool reproduce() const noexcept;

        size_t waterConsumption() const noexcept;

        void draw(int x, int y) const noexcept;
    };
}