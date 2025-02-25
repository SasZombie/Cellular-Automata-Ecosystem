#include "../include/Enviroment.hpp"

bool sas::Water::hasCapacity(size_t consumption) const noexcept
{
    return (capacity >= consumption);
}
void sas::Water::info() const noexcept
{
    std::printf("info\n");
}