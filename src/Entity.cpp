#include "../include/Entity.hpp"

std::pair<size_t, size_t> sas::Entity::getPosition() const noexcept
{
    return pos;
}