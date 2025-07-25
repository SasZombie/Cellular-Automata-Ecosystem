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
        std::shared_ptr<DrawStrategy> drawStrat;
    public:   

        Entity() noexcept = default;
        Entity(const Position& pos) noexcept;
        void setDrawStrategy(std::shared_ptr<DrawStrategy> strat) noexcept;

        std::shared_ptr<DrawStrategy> getDrawStartegy() const noexcept;


        void draw() const noexcept;

        virtual ~Entity() = default;
    };
} // namespace sas