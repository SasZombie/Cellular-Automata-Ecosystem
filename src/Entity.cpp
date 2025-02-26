#include "../include/Entity.hpp"

std::pair<size_t, size_t> sas::Entity::getPosition() const noexcept
{
    return {pos.x, pos.y};
}
void sas::Entity::draw() const noexcept
{
    if (drawStrat)
    {
        drawStrat->draw(pos);
    }
}
void sas::Entity::setDrawStrategy(std::unique_ptr<DrawStrategy> strat) noexcept
{
    drawStrat = std::move(strat);
}