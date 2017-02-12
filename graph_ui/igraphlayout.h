#pragma once
#ifndef GRAPH_UI_IGRAPHLAYOUT_H
#define GRAPH_UI_IGRAPHLAYOUT_H

#include "graph/igraph.hpp"
#include "graph/i2dgraph.hpp"

#include <functional>
#include <memory>

namespace graph_ui
{
    /*! common abstract base class for graph layout algorithms
     */
    template <typename T>
    class IGraphLayout
    {
        public:

            /*! define a size (bounding box) type
             */
            using SizeType = typename std::tuple<int,int>;

            /*! define a function mapping each vertex to its size
                This is useful when laying out the graph, so that the size of vertices can be taken into account.
             */
            using SizeFunctionType = typename std::function<SizeType(const T &vertex)>;

            /*! Layout a graph
             */
            virtual std::unique_ptr<graph::I2DGraph<T>> layout(const graph::IGraph<T> &graph, const SizeFunctionType &size) const = 0;

            /*! \return the minimal margin on the x-axis between vertices
             */
            virtual int getMinXMargin() const = 0;

            /*! \return the maximal margin on the x-axis between vertices
             */
            virtual int getMaxXMargin() const = 0;

            /*! \return the minimal margin on the y-axis between vertices
             */
            virtual int getMinYMargin() const = 0;

            /*! \return the maximal margin on the y-axis between vertices
             */
            virtual int getMaxYMargin() const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // GRAPH_UI_IGRAPHLAYOUT_H
