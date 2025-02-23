#pragma once

#include "Tile.hpp"
#include "matrix.hpp"

#include <functional>
#include <type_traits>
#include <queue>

namespace sas
{
    std::pair<size_t, size_t> generateSeed() noexcept;

    float euclidianDistance2D(size_t x1, size_t y1, size_t x2, size_t y2) noexcept;

    // BFS
    template <typename Func = std::function<bool(const sas::Enviroment &)>>
    std::vector<std::pair<size_t, size_t>> getClosestTileIndexByOccupant(const sas::Matrix<sas::Tile> &board, size_t elemPozX, size_t elemPozY, const sas::Enviroment &env, Func &&condition = [](const sas::Enviroment &)
                                                                                                                                                                              { return true; })
    {

        size_t range = 0;

        std::visit([&](const auto &ptr)
        {
            if constexpr (!std::is_same_v<std::monostate, std::decay_t<decltype(ptr)>>)
            {
                if constexpr (std::is_same_v<sas::Plant, std::decay_t<decltype(*ptr)>>)
                {
                    range = ptr->range();
                }
            }
        }, board(elemPozX, elemPozY).occupant);

        range = 10;

        struct QueueNode
        {
            size_t x, y;
            size_t dist = 0;
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
        std::vector<std::pair<size_t, size_t>> pos;

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
                        {
                            pos.push_back(std::make_pair(current.x, current.y));
                        }
                    }
                } }, board(current.x, current.y).occupant);

            if (current.dist > range)
            {
                return pos;
            }

            // WASD neigh
            for (size_t i = 0; i < 8; ++i)
            {

                size_t newX = current.x + directionsX[i];
                size_t newY = current.y + directionsY[i];
                if (newX < rows && newY < cols && !visited[newX][newY])
                {
                    q.push({newX, newY, current.dist + 1});
                    visited[newX][newY] = true;
                }
            }
        }

        return pos;
    }

} // namespace sas
