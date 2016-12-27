#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP

#include "minimax.hpp"

#include <algorithm>
#include <assert.h>
#include <functional>
#include <iostream>
#include <vector>

namespace game
{
    /*! Tic-tac-toe (also known as noughts and crosses or Xs and Os) is a paper-and-pencil game for two players, X and O, who take turns marking the spaces in a 3×3 grid.
        The player who succeeds in placing three of their marks in a horizontal, vertical, or diagonal row wins the game.
     */
    class TicTacToe final
    {
        public:

            /*! main method for demo tic-tac-toe game with minimax
             */
            static void play()
            {
                /* game evaluation function
                 */
                Minimax<BoardType>::EvaluationFunctionType sFunction = [](const BoardType& b)
                {
                    return score2(b, 0);
                };

                /* next move generation function
                 */
                Minimax<BoardType>::NextMoveFunctionType nFunction = [](const BoardType& b)
                {
                    return nextMoves(b);
                };

                /* init minimax
                 */
                auto minimax = game::Minimax<BoardType>(sFunction, nFunction);

                auto board = empty();

                /* main game loop
                 */
                while(score(board,0) == 0)
                {
                    // calculate next moves
                    board  = minimax.next(board);

                    // print board
                    print(board);

                    if(score(board, 0) != 0)
                    {
                        break;
                    }

                    // process user input
                    auto x = -1;
                    auto y = -1;
                    while(x == -1 || y == -1 || x > 2 || y > 2 || board[x][y]!=-1)
                    {
                        std::cout << "[x,y] : ";
                        std::string temp;
                        getline(std::cin, temp);
                        x = atoi(temp.substr(0,1).c_str());
                        y = atoi(temp.substr(2,1).c_str());
                    }

                    // change board
                    board[x][y] = 1;

                    print(board);

                    std::cout << std::endl;
                }

            }

        private:
            // --- methods ---
            using BoardType = std::vector<std::vector<int>>;

            /* \return empty board
             */
            static BoardType empty()
            {
                BoardType retval;
                for(int i=0; i<3; i++)
                {
                    retval.push_back(std::vector<int>());
                    for(int j=0; j<3; j++)
                    {
                        retval[i].push_back(-1);
                    }
                }
                return retval;
            }

            /* \return a copy of the input board
             */
            static BoardType copy(const BoardType &b)
            {
                assert(b.size() == 3 && b[0].size() == 3 && b[1].size() == 3 && b[2].size() == 3);
                BoardType retval;
                for(int i=0; i<3; i++)
                {
                    retval.push_back(std::vector<int>());
                    for(int j=0; j<3; j++)
                    {
                        retval[i].push_back(b[i][j]);
                    }
                }
                return retval;
            }

            /*! \return next possible boards given the current board
             */
            static std::vector<BoardType> nextMoves(const BoardType &b)
            {
                assert(b.size() == 3 && b[0].size() == 3 && b[1].size() == 3 && b[2].size() == 3);

                // if the game has been won, there are no next moves
                if(score(b,0) != 0)
                {
                    return std::vector<BoardType>();
                }

                // count nof empty spots;
                auto emptySpots = 0;
                for(int i=0; i<3; i++)
                {
                    for(int j=0; j<3; j++)
                    {
                        if(b[i][j] == -1)
                        {
                            emptySpots++;
                        }
                    }
                }

                // if the board is filled, there are no next moves
                if(emptySpots == 0)
                {
                    return std::vector<BoardType>();
                }

                // generate next moves
                std::vector<BoardType> retval;
                auto turn = (emptySpots + 1) % 2;
                for(int i=0; i<3; i++)
                {
                    for(int j=0; j<3; j++)
                    {
                        if(b[i][j] == -1)
                        {
                            auto nextBoard = copy(b);
                            nextBoard[i][j] = turn;
                            retval.push_back(nextBoard);
                        }
                    }
                }

                // return
                return retval;
            }

            /* \return game value, based on which player wins, and how many turns it took them to win
             */
            static int score2(const BoardType& b, const int player)
            {
                auto empty = 0;
                for(int r=0; r<3; r++)
                {
                    for(int c=0; c<3; c++)
                    {
                        if(b[r][c] == -1)
                        {
                            empty++;
                        }
                    }
                }

                auto baseScore = score(b, player);
                if(baseScore == 0)
                {
                    return baseScore;
                }

                return baseScore * 10 + (baseScore > 0 ? empty : -empty);
            }

            /* \return game value, based on which player wins
             */
            static int score(const BoardType &b, const int player)
            {
                assert(b.size() == 3 && b[0].size() == 3 && b[1].size() == 3 && b[2].size() == 3);
                for(int r=0; r<3; r++)
                {
                    // rows
                    if(b[r][0] == b[r][1] && b[r][1] == b[r][2] && b[r][0] != -1)
                    {
                        return b[r][0] == player ? 10 : -10;
                    }
                    // cols
                    if(b[0][r] == b[1][r] && b[1][r] == b[2][r] && b[0][r] != -1)
                    {
                        return b[0][r] == player ? 10 : -10;
                    }
                }
                // diagonal
                if(b[0][0] == b[1][1] && b[1][1] == b[2][2] && b[0][0] != -1)
                {
                    return b[0][0] == player ? 10 : -10;
                }
                if(b[0][2] == b[1][1] && b[1][1] == b[2][0] && b[0][2] != -1)
                {
                    return b[0][2] == player ? 10 : -10;
                }
                // default
                return 0;
            }

            /*! print the game board
             */
            static void print(const BoardType &b)
            {
                assert(b.size() == 3 && b[0].size() == 3 && b[1].size() == 3 && b[2].size() == 3);
                for(int i=0; i<3; i++)
                {
                    for(int j=0; j<3; j++)
                    {
                        std::cout << (b[i][j] == 0 ? "O" : (b[i][j] == 1 ? "X" : "_"));
                    }
                    std::cout << std::endl << std::endl;
                }
            }

            // --- members ---
    };
}

#endif // TICTACTOE_HPP