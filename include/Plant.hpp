#pragma once
#include <cstddef>

namespace sas
{
    class Plant
    {
    public:
        virtual bool reproduce() const noexcept = 0;
        virtual size_t waterConsumption() const noexcept = 0; 
        virtual ~Plant() = default;

    };

    class Tree: public Plant
    {
    public:
        virtual bool reproduce() const noexcept;

        virtual size_t waterConsumption() const noexcept;
    };

    class Flower: public Plant
    {
    public:
        virtual bool reproduce() const noexcept;

        virtual size_t waterConsumption() const noexcept;
    };

    class Weed: public Plant
    {
    public:
        virtual bool reproduce() const noexcept;

        virtual size_t waterConsumption() const noexcept;
    };
}