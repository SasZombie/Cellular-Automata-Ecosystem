#include <vector>
#include <iostream>
#include "tile.hpp"

int main()
{
    sas::Tile t(sas::TileType::GRASS);

    t.plant = std::make_unique<sas::Tree>();

    std::cout << t.plant->reproduce();

    t.plant = std::make_unique<sas::Flower>();

    std::cout << t.plant->reproduce();

}