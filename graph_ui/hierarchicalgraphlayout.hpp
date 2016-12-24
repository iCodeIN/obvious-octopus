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
    /*! Layered graph drawing or hierarchical graph drawing is a type of graph drawing in which the vertices of a directed graph are drawn in horizontal rows or layers with the edges generally directed downwards.
        It is also known as Sugiyama-style graph drawing after Kozo Sugiyama, who first developed this drawing style.
        The ideal form for a layered drawing would be an upward planar drawing, in which all edges are oriented in a consistent direction and no pairs of edges cross.
        However, graphs often contain cycles, minimizing the number of inconsistently-oriented edges is NP-hard, and minimizing the number of crossings is also NP-hard,
        so layered graph drawing systems typically apply a sequence of heuristics that reduce these types of flaws in the drawing without guaranteeing to find a drawing with the minimum number of flaws.
     */
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