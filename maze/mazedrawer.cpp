#include "mazedrawer.h"

#include "maze.h"

#include "general/direction.h"

#include "svg/line.hpp"
#include "svg/rectangle.hpp"
#include "svg/svg.hpp"

using namespace General;

std::unique_ptr<SVG::SVG> MazeDrawer::render(const Maze &maze)
{
    auto img = std::unique_ptr<SVG::SVG>(new SVG::SVG());
    img.get()->setViewBox(0,0,maze.width()*10, maze.height()*10);

    // white background
    auto background = std::unique_ptr<SVG::Rectangle>(new SVG::Rectangle(0, 0, maze.width()*10, maze.height()*10));
    background.get()->setStrokeWidth(0);
    background.get()->setFillOpacity(1.0);
    background.get()->setFill(255,255,255);
    img.get()->add(std::move(background));

    for(int i=0; i<maze.width(); i++)
    {
        for(int j=0; j<maze.height(); j++)
        {
            auto x = i * 10;
            auto y = j * 10;
            Maze::PointType p = std::make_pair(i,j);

            // draw north wall
            if(maze.hasWall(p, Direction::NORTH))
            {
                auto line = std::unique_ptr<SVG::Line>(new SVG::Line(x,y,x+10,y));
                line.get()->setStrokeWidth(0.2);
                line.get()->setStrokeOpacity(1.0);
                img.get()->add(std::move(line));
            }
            // draw east wall
            if(maze.hasWall(p, Direction::EAST))
            {
                auto line = std::unique_ptr<SVG::Line>(new SVG::Line(x+10,y, x+10,y+10));
                line.get()->setStrokeWidth(0.2);
                line.get()->setStrokeOpacity(1.0);
                img.get()->add(std::move(line));
            }
            // draw south wall
            if(maze.hasWall(p, Direction::SOUTH))
            {
                auto line = std::unique_ptr<SVG::Line>(new SVG::Line(x+10,y+10,x,y+10));
                line.get()->setStrokeWidth(0.2);
                line.get()->setStrokeOpacity(1.0);
                img.get()->add(std::move(line));
            }
            // draw west wall
            if(maze.hasWall(p, Direction::WEST))
            {
                auto line = std::unique_ptr<SVG::Line>(new SVG::Line(x,y+10,x,y));
                line.get()->setStrokeWidth(1.0);
                line.get()->setStrokeOpacity(1.0);
                img.get()->add(std::move(line));
            }
        }
    }
    return img;
}
