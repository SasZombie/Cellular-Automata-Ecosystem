#pragma once
#include <tuple>
#include <string>
#include <memory>
#include "DrawStrategy.hpp"

namespace sas
{
   
    class Entity
    {
    public:
        
        Position pos;
        std::pair<size_t, size_t> getPosition() const noexcept;

    private:
        //This is private because we call move in the setter
        std::unique_ptr<DrawStrategy> drawStrat;
    public:   
        void setDrawStrategy(std::unique_ptr<DrawStrategy> strat) noexcept;

        void draw() const noexcept;

        virtual ~Entity() = default;
    };
} // namespace sas