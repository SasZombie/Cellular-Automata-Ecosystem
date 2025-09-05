#pragma once
#include "Enviroment.hpp"
#include "Matrix.hpp"
#include "Plant.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include "GameState.hpp"


namespace sas
{

    class Game
    {
    //Looks weird but I do this for sturcture alignment and I feel weird if I dont
    public:        
        GameState currentState = GameState::MENU;
        size_t seed;
    private:
        Matrix<Tile> board;

        std::vector<std::unique_ptr<Enviroment>> enviroment;
        std::vector<std::unique_ptr<Plant>> plants;

        StaticGrid enviromentGrid;
        DynamicGrid plantGrid;

        
    public:
        Game(size_t n_seed = 1);
        
        void setUp(size_t n_seed);
        void passTime() noexcept;
        const std::vector<std::unique_ptr<Plant>>& plantsRef() const noexcept;
        const std::vector<std::unique_ptr<Enviroment>>& enviromentRef() const noexcept;
        const Matrix<Tile>& boardRef() const noexcept;
        
        ~Game() = default;

    private:
        void clearAll() noexcept;
    };

} // namespace sas
