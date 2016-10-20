#include "maze.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

using namespace General;

Maze::Maze(int w, int h)
    : m_width(w)
    , m_height(h)
{
    assert(w >= 0 && h >= 0);

    // seed random
    srand(time(0));

    // make empty maze, all walls intact
    for(int i=0; i<w; i++)
    {
        std::vector<Cell> row;
        for(int j=0; j<h; j++)
        {
            row.push_back(Maze::Cell(std::make_pair(i,j)));
        }
        m_cells.push_back(row);
    }

    // pick starting cell
    Maze::PointType curr = std::make_pair<int,int>(rand() % w, rand() % h);

    // keep stack
    std::vector<Maze::PointType> toVisit;
    toVisit.push_back(curr);

    // build maze
    while(!toVisit.empty())
    {
        Maze::PointType curr = toVisit[toVisit.size() - 1];

        // choose random neigbour, or backtrack
        auto nbs = intactNeighbours(curr);
        if(nbs.empty())
        {
            toVisit.erase(toVisit.begin() + toVisit.size() - 1);
        }
        else
        {
            // select random intact neighbour
            auto &nb = nbs[rand() % nbs.size()];

            // break down walls
            visit(curr, nb);

            // add to stack
            toVisit.push_back(nb);
        }
    }
}

int Maze::width() const
{
    return m_width;
}

int Maze::height() const
{
    return m_height;
}

bool Maze::hasWall(const PointType &p, Direction d) const
{
    assert(p.first >= 0 && p.first < m_width && p.second >= 0 && p.second < m_height);
    return m_cells[p.first][p.second].hasWall(d);
}

void Maze::visit(const Maze::PointType& from, const Maze::PointType& to)
{
    assert(from.first >= 0 && from.first < m_width && from.second >= 0 && from.second < m_height);
    assert(to.first >= 0 && to.first < m_width && to.second >= 0 && to.second < m_height);
    //
    // T
    // |
    // F
    //
    if(from.first == to.first && from.second == to.second + 1)
    {
        m_cells[from.first][from.second].setWall(Direction::NORTH, false);
        m_cells[to.first][to.second].setWall(Direction::SOUTH, false);
    }
    //
    // F
    // |
    // T
    //
    if(from.first == to.first && from.second == to.second - 1)
    {
        m_cells[from.first][from.second].setWall(Direction::SOUTH, false);
        m_cells[to.first][to.second].setWall(Direction::NORTH, false);
    }
    //
    // F - T
    //
    if(from.second == to.second && from.first == to.first - 1)
    {
        m_cells[from.first][from.second].setWall(Direction::EAST, false);
        m_cells[to.first][to.second].setWall(Direction::WEST, false);
    }
    //
    // T - F
    //
    if(from.second == to.second && from.first == to.first + 1)
    {
        m_cells[from.first][from.second].setWall(Direction::WEST, false);
        m_cells[to.first][to.second].setWall(Direction::EAST, false);
    }
}

std::vector<Maze::PointType> Maze::intactNeighbours(const Maze::PointType& p)
{
    assert(p.first >= 0 && p.first < m_width && p.second >= 0 && p.second < m_height);

    // define 'absolute value' function
    auto abs = [](int i)
    {
        return i < 0 ? -i : i;
    };

    // check all neighbours within manhattan distance of 1
    std::vector<Maze::PointType> nbs;
    for(int i=-1; i<=1; i++)
    {
        for(int j=-1; j<=1; j++)
        {
            if(abs(i)==abs(j))
            {
                continue;
            }
            int x = p.first + i;
            int y = p.second + j;
            if(x >= m_width || x < 0)
            {
                continue;
            }
            if(y >= m_height || y < 0)
            {
                continue;
            }
            auto c = m_cells[x][y];
            if(c.hasWall(Direction::NORTH) && c.hasWall(Direction::EAST) && c.hasWall(Direction::SOUTH) && c.hasWall(Direction::WEST))
            {
                nbs.push_back(std::make_pair(x,y));
            }
        }
    }
    return nbs;
}
