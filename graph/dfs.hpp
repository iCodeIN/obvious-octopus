#pragma once
#ifndef GRAPH_DFS_HPP
#define GRAPH_DFS_HPP

#include "graph/itree.hpp"
#include "graph/igraph.hpp"
#include "graph/adjecencylisttree.hpp"

#include <memory>
#include <stack>

namespace graph
{
    namespace algorithm
    {
        /*! Depth-first search (DFS) is an algorithm for traversing or searching tree or graph data structures.
            One starts at the root (selecting some arbitrary node as the root in the case of a graph) and explores as far as possible along each branch before backtracking.
            A version of depth-first search was investigated in the 19th century by French mathematician Charles Pierre Trémaux as a strategy for solving mazes.
         */
        template <typename T>
        static std::unique_ptr<ITree<T>> dfs(const IGraph<T> &graph, const T& root)
        {
            auto tree = new AdjecencyListTree<T>();

            // dfs build loop
            std::stack<T> s;
            s.push(root);
            tree->insertVertex(root);
            while(!s.empty())
            {
                auto vertex = s.top();
                s.pop();

                if(tree->hasVertex(vertex) && !tree->outgoing(vertex).empty())
                {
                    continue;
                }

                for(auto nextVertex : graph.outgoing(vertex))
                {
                    if(tree->hasVertex(nextVertex) && !tree->incoming(nextVertex).empty())
                    {
                        continue;
                    }
                    if(tree->hasVertex(nextVertex) && !tree->outgoing(nextVertex).empty())
                    {
                        continue;
                    }

                    tree->insertVertex(nextVertex);
                    tree->insertEdge(vertex, nextVertex);
                    s.push(nextVertex);
                }
            }

            return std::unique_ptr<ITree<T>>(tree);
        }
    }
}

#endif // GRAPH_DFS_HPP