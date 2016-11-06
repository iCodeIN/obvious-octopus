#pragma once
#ifndef I2DGRAPH_HPP
#define I2DGRAPH_HPP

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

            /*! define PointType as std::pair<int,int>
             */
            using PointType = std::pair<int,int>;

            /*! \return the PointType at which the given vertex is located
             */
            virtual const PointType getVertexPoint(const T &vertex) const = 0;

            /*! \return the path of PointTypes at which the edge is located
             */
            virtual std::vector<PointType> const getEdgePoints(const T& source, const T& target) const = 0;

        private:
    };
}

#endif // I2DGRAPH_HPP