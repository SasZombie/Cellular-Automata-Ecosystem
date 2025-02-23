#pragma once

#include "Tile.hpp"
#include "matrix.hpp"

#include <functional>
#include <type_traits>
#include <queue>


namespace sas
{
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dist;
    size_t reproducingGeneratedSeed;
    
    void generateSeed();
    
    void generateSeed() {
        reproducingGeneratedSeed = static_cast<size_t>(dist(gen) * 21473);
    }
    
    float euclidianDistance2D(const size_t &x1, const size_t &y1, const size_t &x2, const size_t &y2) {
        return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    }
    
    // BFS
    template <typename Func = std::function<bool(const sas::Enviroment&)>>
    std::optional<std::pair<size_t, size_t>> getClosestTileIndexByOccupant(const sas::Matrix<sas::Tile> &board, size_t elemPozX, size_t elemPozY,
                                                                           const sas::Enviroment &env, Func&& condition = [](const sas::Enviroment&) { return true; })
    {

        struct QueueNode
        {
            size_t x, y;
        };

        const size_t rows = board.getRows();
        const size_t cols = board.getCols();

        constexpr int directionsX[] = {-1, 1, 0, 0, -1, 1, -1, 1};
        constexpr int directionsY[] = {0, 0, -1, 1, -1, 1, 1, -1};

        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

        std::queue<QueueNode> q;
        q.push({elemPozX, elemPozY});
        visited[elemPozX][elemPozY] = true;

        std::optional<std::pair<size_t, size_t>> result;

        while (!q.empty())
        {
            const QueueNode current = q.front();
            q.pop();

            std::visit([&](const auto &ptr)
                       {
                if constexpr (!std::is_same_v<std::monostate, std::decay_t<decltype(ptr)>>)
                {
                    if constexpr ((std::is_same_v<std::decay_t<decltype(env)>, std::decay_t<decltype(*ptr)>> ||
                        std::is_same_v<std::decay_t<decltype(env)>, std::decay_t<decltype(*ptr)>>))
                    {
                        if(condition(*ptr))
                            result = std::make_pair(current.x, current.y);
                    }
                } }, board(current.x, current.y).occupant);

            if (result)
            {
                return result;
            }

            //WASD neigh
            for (size_t i = 0; i < 8; ++i)
            {
    
                size_t newX = current.x + directionsX[i];
                size_t newY = current.y + directionsY[i];
                if (newX < rows && newY < cols && !visited[newX][newY])
                {
                    q.push({newX, newY});
                    visited[newX][newY] = true;
                }
            }
        }

        return std::nullopt;
    }

} // namespace sas
