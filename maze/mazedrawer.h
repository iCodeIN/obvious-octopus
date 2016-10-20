#pragma once
#ifndef MAZEDRAWER_H
#define MAZEDRAWER_H

#include <memory>

namespace SVG
{
    class SVG;
}

class Maze;

/*! This class is responsible for drawing a Maze.
	 Thereby separating UI-logic from Maze-generation logic.
 */
class MazeDrawer final
{
    public:
        /*! Prohibit construction of MazeDrawer. This class offers only static methods.
         */
        explicit MazeDrawer() = delete;

        /*! \return the SVG image representing the given Maze
        	 \param[in] maze	the Maze to be drawn
         */
        static std::unique_ptr<SVG::SVG> render(const Maze &maze);
};

#endif // MAZEDRAWER_H
