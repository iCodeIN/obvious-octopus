#include "svg/line.hpp"
#include "svg/rectangle.hpp"
#include "svg/svg.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

std::vector<std::vector<int>> maze;

/*! initialize an empty maze of size w by h
 */
void initMaze(int w, int h)
{
    maze.clear();
    for(int i=0; i<w; i++)
    {
        std::vector<int> row;
        for(int j=0; j<h; j++)
        {
            row.push_back(2 * 3 * 5 * 7);
        }
        maze.push_back(row);
    }
}

/*! \return true iff the cell at position [x,y] has a wall at the given direction
    0 -> NORTH
    1 -> EAST
    2 -> SOUTH
    3 -> WEST
 */
bool hasWall(int x, int y, int dir)
{
    int c = maze[x][y];
    if(dir == 0)
    {
        return (c % 2) == 0;
    }
    if(dir == 1)
    {
        return (c % 3) == 0;
    }
    if(dir == 2)
    {
        return (c % 5) == 0;
    }
    if(dir == 3)
    {
        return (c % 7) == 0;
    }
    return false;
}

/*! \return true iff the cell at position [x,y] is intact (i.e. has four walls)
 */
bool isIntact(int x, int y)
{
    return hasWall(x, y, 0) && hasWall(x, y, 1) && hasWall(x, y, 2) && hasWall(x, y, 3);
}

std::vector<std::pair<int,int>> intactNeighbours(int x, int y)
{
    std::vector<std::pair<int,int>> retval;
    if( x -1 >= 0 && isIntact(x-1,y))
    {
        retval.push_back(std::make_pair(x-1,y));
    }
    if( x + 1 < maze.size() && isIntact(x+1,y))
    {
        retval.push_back(std::make_pair(x + 1,y));
    }
    if( y -1 >= 0 && isIntact(x,y-1))
    {
        retval.push_back(std::make_pair(x,y-1));
    }
    if( y + 1 < maze[0].size() && isIntact(x,y + 1))
    {
        retval.push_back(std::make_pair(x,y + 1));
    }
    return retval;
}

void connect(int x0, int y0, int x1, int y1)
{
    if(y0 == y1)
    {
        if(x0 + 1 == x1)
        {
            maze[x0][y0] /= 3;
            maze[x1][y0] /= 7;
        }
        else if(x1 + 1 == x0)
        {
            maze[x1][y0] /= 3;
            maze[x0][y0] /= 7;
        }
    }
    else if(x0 == x1)
    {
        if(y0 + 1 == y1)
        {
            maze[x0][y0] /= 5;
            maze[x0][y1] /= 2;
        }
        else if(y1 + 1 == y0)
        {
            maze[x0][y1] /= 5;
            maze[x0][y0] /= 2;
        }
    }
}

std::unique_ptr<SVG::SVG> render()
{
    int w = maze.size();
    int h = maze[0].size();

    auto img = std::unique_ptr<SVG::SVG>(new SVG::SVG());
    img.get()->setViewBox(0,0, w * 10, h * 10);

    // white background
    auto background = std::unique_ptr<SVG::Rectangle>(new SVG::Rectangle(0, 0, w * 10, h * 10));
    background.get()->setStrokeWidth(0);
    background.get()->setFillOpacity(1.0);
    background.get()->setFill(255,255,255);
    img.get()->add(std::move(background));

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            auto x = i * 10;
            auto y = j * 10;
            auto p = std::make_pair(i,j);

            // draw north wall
            if(hasWall(p.first, p.second, 0))
            {
                auto line = std::unique_ptr<SVG::Line>(new SVG::Line(x,y,x+10,y));
                line.get()->setStrokeWidth(0.2);
                line.get()->setStrokeOpacity(1.0);
                img.get()->add(std::move(line));
            }
            // draw east wall
            if(hasWall(p.first, p.second, 1))
            {
                auto line = std::unique_ptr<SVG::Line>(new SVG::Line(x+10,y, x+10,y+10));
                line.get()->setStrokeWidth(0.2);
                line.get()->setStrokeOpacity(1.0);
                img.get()->add(std::move(line));
            }
            // draw south wall
            if(hasWall(p.first, p.second, 2))
            {
                auto line = std::unique_ptr<SVG::Line>(new SVG::Line(x+10,y+10,x,y+10));
                line.get()->setStrokeWidth(0.2);
                line.get()->setStrokeOpacity(1.0);
                img.get()->add(std::move(line));
            }
            // draw west wall
            if(hasWall(p.first, p.second, 3))
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

int main()
{
    auto w = 50;
    auto h = 50;

    // 0. seed random
    srand(time(0));

    // 1. initialize empty maze
    initMaze(w, h);

    // 2. choose random starting cell
    auto curr = std::make_pair(rand() % w, rand() % h);

    // 3. main loop
    std::vector<std::pair<int,int>> toVisit;
    toVisit.push_back(curr);

    while(!toVisit.empty())
    {
        curr = toVisit[toVisit.size() - 1];
        auto nbs = intactNeighbours(curr.first, curr.second);
        if(nbs.size() == 0)
        {
            // backtracking
            toVisit.erase(toVisit.begin() + toVisit.size() - 1);
        }
        else
        {
            // choose random next neighbour
            auto nxt = nbs[rand() % nbs.size()];
            connect(curr.first, curr.second, nxt.first, nxt.second);
            toVisit.push_back(nxt);
        }
    }

    // 4. visualization
    auto svgPtr = render();

    // 5. persist to file
    std::cout << *svgPtr << std::endl;

    // return
    return 0;
}