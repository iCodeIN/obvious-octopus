#pragma once
#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include "graph/graph.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace game
{
    /*! Minimax (sometimes MinMax or MM[1]) is a decision rule used in decision theory, game theory,
        statistics and philosophy for minimizing the possible loss for a worst case (maximum loss) scenario.
        Originally formulated for two-player zero-sum game theory, covering both the cases where players take alternate moves and those where they make simultaneous moves,
        it has also been extended to more complex games and to general decision-making in the presence of uncertainty.
     */
    template <typename T>
    class Minimax final
    {
        public:
            using GamePlayType = graph::Graph<T>;
            using EvaluationFunctionType = std::function<int(const GamePlayType &g)>;
            using nextMoveFunctionType = std::function<std::vector<T>(const T &position)>;

            std::unique_ptr<GamePlayType> build(const T &startingPosition)
            {

            }

        private:
    };
}

#endif // MINIMAX_HPP