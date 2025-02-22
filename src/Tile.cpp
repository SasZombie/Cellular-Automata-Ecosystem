#include "../include/Tile.hpp"

void sas::Tile::addPlant(std::unique_ptr<sas::Plant> plt) noexcept
{
    this->occupant = std::move(plt);
}

void sas::Tile::addEnviroment(std::unique_ptr<sas::Enviroment> env) noexcept
{
    this->occupant = std::move(env);
}
