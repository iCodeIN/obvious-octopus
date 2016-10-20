#ifndef MAZE_H
#define MAZE_H

#include "general/direction.h"

#include <vector>

namespace SVG
{
    class SVG;
}

/*! The algorithm used to generate this Maze is a randomized version of the depth-first search algorithm. Implemented with a stack, this approach is one of the simplest ways to generate a maze using a computer.
	 Consider the space for a maze being a large grid of cells (like a large chess board), each cell starting with four walls.
	 Starting from a random cell, the computer then selects a random neighbouring cell that has not yet been visited. The computer removes the wall between the two cells and marks the new cell as visited,
	 and adds it to the stack to facilitate backtracking. The computer continues this process, with a cell that has no unvisited neighbours being considered a dead-end.
	 When at a dead-end it backtracks through the path until it reaches a cell with an unvisited neighbour, continuing the path generation by visiting this new, unvisited cell (creating a new junction).
	 This process continues until every cell has been visited, causing the computer to backtrack all the way back to the beginning cell. We can be sure every cell is visited.
*/
class Maze final
{
    public:
        /*! Defines the type used for coordinates within a Maze
         */
        using PointType = std::pair<int,int>;

        /*! Constructor
        	 \param[in] w	width of the (rectangular) Maze
        	 \param[in] h	height of the (rectangular) Maze
         */
        explicit Maze(int w, int h);

        /*! Default destructor
         */
        virtual ~Maze() = default;

        /*! Prohibit const copy constructor
         */
        Maze(const Maze&) = delete;

        /*! Prohibit copy constructor
         */
        Maze(Maze&) = delete;

        /*! Prohibit const assignment operator
         */
        void operator=(const Maze&) = delete;

        /*! Prohibit assignment operator
         */
        void operator=(Maze&) = delete;

        /*! \return the width of the (rectangular) Maze
         */
        int width() const;

        /*! \return the height of the (rectangular) Maze
         */
        int height() const;

        /*! \return true if this Maze has a wall at given location in a given direction
        	 \param[in] p	the location within the maze
        	 \param[in] d	the direction
         */
        bool hasWall(const PointType &p, General::Direction d) const;

    private:
        // --- Methods ---
        std::vector<PointType> intactNeighbours(const PointType& p);
        void visit(const PointType& from, const PointType& to);

        // --- Members ---
        class Cell final
        {
            public:
                explicit Cell(PointType p)
                {
                    m_location = p;
                }
                virtual ~Cell() = default;

                PointType getLocation() const
                {
                    return m_location;
                }

                bool hasWall(General::Direction d) const
                {
                    return m_walls[(int)d];
                }
                void setWall(General::Direction d, bool aFlag)
                {
                    m_walls[(int)d] = aFlag;
                }

            private:
                PointType m_location;								//!< location of the cell in the Maze
                bool m_walls[4] = {true, true, true, true}; 	//!< keeps track of which walls are raised
        };

        std::vector<std::vector<Cell>> m_cells;	//!< cells
        int m_width;										//!< width
        int m_height;										//!< height
};

#endif // MAZE_H
