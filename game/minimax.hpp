#pragma once
#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include <algorithm>
#include <assert.h>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace game
{
    /*! Minimax (sometimes MinMax or MM) is a decision rule used in decision theory, game theory,
        statistics and philosophy for minimizing the possible loss for a worst case (maximum loss) scenario.
        Originally formulated for two-player zero-sum game theory, covering both the cases where players take alternate moves and those where they make simultaneous moves,
        it has also been extended to more complex games and to general decision-making in the presence of uncertainty.
     */
    template <typename T>
    class Minimax final
    {
        public:
            /*! define the function to evaluate game positions
             */
            using EvaluationFunctionType = std::function<int(const T& position)>;
            /*! define the function to generate the next possible game-positions given a starting position
             */
            using NextMoveFunctionType = std::function<std::vector<T>(const T& position)>;

            /*! default constructor
             */
            Minimax(EvaluationFunctionType &evalFunction, NextMoveFunctionType &nextMoveFunction)
                : m_eval(evalFunction)
                , m_nextMove(nextMoveFunction)
                , m_nofPlayers(2)
            {
            }

            /*! default destructor
             */
            virtual ~Minimax() = default;

            /*! Prohibit const copy constructor
             */
            Minimax(const Minimax&) = delete;

            /*! Prohibit copy constructor
             */
            Minimax(Minimax&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Minimax&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Minimax&) = delete;

            /*! \return the next (optimal) position given the starting positions
                This method uses minimax, and alpha-beta pruning.
             */
            T next(const T &startingPosition)
            {
                // insert starting position into memory
                m_root = std::unique_ptr<Node>(new Node(startingPosition,this));

                // perform search, with alpha/beta pruning
                m_root->alphabeta();

                // get next (optimal) move
                return m_root->next();
            }

        private:
            // --- methods ---

            // --- members ---
            class Node;
            class Node
            {
                public:
                    Node(const T& position, Minimax* minimax)
                        : m_game(position)
                        , m_minimax(minimax)
                        , m_parent(this)
                    {
                        assert(minimax);
                    }

                    Node* insert(const T& position)
                    {
                        m_children.push_back(std::unique_ptr<Node>(new Node(position, m_minimax)));
                        m_children[m_children.size()-1]->m_parent = this;
                        return m_children[m_children.size()-1].get();
                    }

                    std::vector<Node*> leaves()
                    {
                        std::vector<Node*> retval;
                        recursiveLeaves(this, retval);
                        return retval;
                    }

                    int alphabeta()
                    {
                        return recursiveaAlphaBeta(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 0);
                    }

                    T next()
                    {
                        for(auto &leaf : leaves())
                        {
                            if(isConstantLeaf(leaf))
                            {
                                return pathToRoot(leaf)[0]->m_game;
                            }
                        }
                        assert(false);
                    }

                private:
                    // --- methods ---

                    std::vector<Node*> pathToRoot(Node* n) const
                    {
                        std::vector<Node*> retval;
                        while(n->m_parent != n->m_parent->m_parent)
                        {
                            retval.push_back(n);
                            n = n->m_parent;
                        }
                        retval.push_back(n);
                        std::reverse(retval.begin(), retval.end());
                        return retval;
                    }

                    bool isConstantLeaf(Node* n) const
                    {
                        assert(n);
                        while(n->m_parent != n->m_parent->m_parent && n->m_value == n->m_parent->m_value)
                        {
                            assert(n->m_parent);
                            n = n->m_parent;
                        }
                        assert(n && n->m_parent && n->m_parent->m_parent);
                        return n->m_parent == n->m_parent->m_parent && n->m_value == n->m_parent->m_value;
                    }

                    void recursiveLeaves(Node* n, std::vector<Node*>& v)
                    {
                        assert(n);
                        if(n->m_children.empty())
                        {
                            v.push_back(n);
                        }
                        else
                        {
                            for(auto &child : n->m_children)
                            {
                                recursiveLeaves(child.get(), v);
                            }
                        }
                    }

                    int recursiveaAlphaBeta(int alpha, int beta, int turn)
                    {
                        assert(m_minimax);
                        auto nextMoves = m_minimax->m_nextMove(m_game);
                        if(nextMoves.empty())
                        {
                            m_value = m_minimax->m_eval(m_game);
                            return m_value;
                        }

                        // maximizing player
                        if(turn == 0)
                        {
                            auto v = std::numeric_limits<int>::min();
                            for(auto &nextMove : nextMoves)
                            {
                                // update tree
                                auto childNodePtr = insert(nextMove);

                                // default alpha-beta pruning
                                v = std::max(v, childNodePtr->recursiveaAlphaBeta(alpha, beta,(turn + 1) % m_minimax->m_nofPlayers));
                                alpha = std::max(alpha, v);
                                if(beta <= alpha)
                                {
                                    break;
                                }
                            }
                            m_value = v;
                            return v;
                        }
                        // minimizing player
                        else
                        {
                            auto v = std::numeric_limits<int>::max();
                            for(auto &nextMove : nextMoves)
                            {
                                // update tree
                                auto childNodePtr = insert(nextMove);
                                assert(childNodePtr);

                                // default alpha-beta pruning
                                v = std::min(v, childNodePtr->recursiveaAlphaBeta(alpha, beta,(turn + 1) % m_minimax->m_nofPlayers));
                                beta = std::min(beta, v);
                                if(beta <= alpha)
                                {
                                    break;
                                }
                            }
                            m_value = v;
                            return v;
                        }
                    }

                    // --- members ---
                    T                                       m_game;         // current position in the game
                    std::vector<std::unique_ptr<Node>>      m_children;     // child nodes
                    Minimax*                                m_minimax;      // overlying minimax class (used for getting next moves, and evaluating game positions)
                    Node*                                   m_parent;       // parent node (pointer to self in case of root)
                    int                                     m_value;
            };

            std::unique_ptr<Node>               m_root;
            EvaluationFunctionType&             m_eval;
            NextMoveFunctionType&               m_nextMove;
            int                                 m_nofPlayers;
    };
}

#endif // MINIMAX_HPP