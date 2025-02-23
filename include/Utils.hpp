#pragma once

#include "Tile.hpp"
#include "matrix.hpp"

#include <type_traits>
#include <queue>

namespace sas
{
    // BFS
    std::optional<std::pair<size_t, size_t>> getClosestTileIndexByOccupant(const sas::Matrix<sas::Tile> &board, size_t elemPozX, size_t elemPozY,
                                                                           const sas::Enviroment &env)
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
                        if(env.hasCapacity(1))
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
