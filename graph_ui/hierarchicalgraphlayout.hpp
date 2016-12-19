#pragma once
#ifndef HIERARCHICAL_GRAPH_LAYOUT_HPP
#define HIERARCHICAL_GRAPH_LAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"

#include "graph/cyclefinder.hpp"
#include "graph/i2dgraph.hpp"
#include "graph/igraph.hpp"

#include <memory>

namespace graph_ui
{
    template <typename T>
    class HierarchicalGraphLayout : public AbstractGraphLayout<T>
    {
        public:
            // --- AbstractGraphLayout ---
            virtual std::unique_ptr<graph::I2DGraph<T>> layout(const graph::IGraph<T> &graph, const typename AbstractGraphLayout<T>::SizeFunctionType &size) const override
            {
                assert(!graph::CycleFinder<T>::hasCycle(graph));

                // duplicate graph into I2DGraph

                // set y coordinate based on BFS depth

                // insert dummy vertices to ensure layout properties are methods

                // layout graph, layer by layer

                // return
            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // HIERARCHICAL_GRAPH_LAYOUT_HPP