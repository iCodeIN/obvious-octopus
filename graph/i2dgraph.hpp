#pragma once
#ifndef GRAPH_I2DGRAPH_HPP
#define GRAPH_I2DGRAPH_HPP

#include "igraph.hpp"

#include <vector>
#include <utility>

namespace graph
{
    /*! IGraph with further 2D point data for each vertex, and a path (i.e. vector<pair<int,int>>) for each vertex
     */
    template <typename T>
    class I2DGraph : public IGraph<T>
    {
        public:
            /*! default constructor
             */
            explicit I2DGraph() = default;

            /*! default destructor
             */
            virtual ~I2DGraph() = default;

            /*! Prohibit const copy constructor
             */
            I2DGraph(const I2DGraph&) = delete;

            /*! Prohibit copy constructor
             */
            I2DGraph(I2DGraph&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const I2DGraph&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(I2DGraph&) = delete;

            /*! \return the PointType at which the given vertex is located
             */
            virtual const std::pair<int,int> getVertexPoint(const T &vertex) const = 0;

            /*! \return the path of PointTypes at which the edge is located
             */
            virtual std::vector<std::pair<int,int>> const getEdgePoints(const T& source, const T& target) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // GRAPH_I2DGRAPH_HPP