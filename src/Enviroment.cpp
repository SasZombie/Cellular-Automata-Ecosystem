#include "../include/Enviroment.hpp"
#include <iostream>

bool sas::Water::hasCapacity(size_t consumption) const noexcept
{
    return (capacity >= consumption);
}
void sas::Water::info() const noexcept
{
    std::cout << "info\n";
}