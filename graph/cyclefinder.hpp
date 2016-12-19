#pragma once
#ifndef CYCLEFINDER_HPP
#define CYCLEFINDER_HPP

#include "bfs.hpp"
#include "igraph.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include <iostream> // debug

namespace graph
{
    /*! The existence of a cycle in directed and undirected graphs can be determined by whether depth-first search (DFS) finds an edge that points to an ancestor of the current vertex (it contains a back edge).
        In an undirected graph, finding any already visited vertex will indicate a back edge. All the back edges which DFS skips over are part of cycles.
        In the case of undirected graphs, only O(n) time is required to find a cycle in an n-vertex graph, since at most n − 1 edges can be tree edges.
     */
    template <typename T>
    class CycleFinder
    {
        public:

            /*! Define a cycle
                A cycle consists of a sequence of vertices starting and ending at the same vertex, with each two consecutive vertices in the sequence adjacent to each other in the graph.
                In a directed graph, each edge must be traversed by the walk consistently with its direction:
                the edge must be oriented from the earlier of two consecutive vertices to the later of the two vertices in the sequence.
                The choice of starting vertex is not important: traversing the same cyclic sequence of edges from different starting vertices produces the same closed walk.
             */
            using Cycle = std::vector<T>;

            /*! \return a set of all cycles in the given IGraph
                \param[in] graph    the IGraph to be scanned
             */
            static const std::set<Cycle> cycles(const IGraph<T> &graph)
            {
                std::set<Cycle> retval;

                // make a todo stack of vertices
                std::stack<T> verticesToDo;
                for(auto &vertex : graph.vertices())
                {
                    verticesToDo.push(vertex);
                }

                // main loop
                while(!verticesToDo.empty())
                {
                    // current vertex
                    auto &root = verticesToDo.top();
                    verticesToDo.pop();

                    // apply bfs to find possible cycle
                    auto tree = bfs<T>(graph, root);

                    // if a leaf in the tree is connected to the root, we have a cycle
                    for(auto leaf : tree->leaves())
                    {

                        if(graph.hasEdge(leaf, root))
                        {
                            // rebuild cycle
                            Cycle c;
                            while(leaf != root)
                            {
                                c.push_back(leaf);
                                leaf = tree->parent(leaf);
                            }
                            c.push_back(leaf);

                            // apply rotation to standardize cycle
                            std::rotate(c.begin(), std::min_element(c.begin(), c.end()), c.end());

                            // put cycle in retval
                            retval.insert(c);
                        }
                    }
                }

                // return
                return retval;
            }

            /*! \return true iff the IGraph contains at least one cycle
             */
            static bool hasCycle(const IGraph<T>& graph)
            {
                return !cycles(graph).empty();
            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // CYCLEFINDER_HPP