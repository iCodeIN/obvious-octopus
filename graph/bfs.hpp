#pragma once
#ifndef BFS_HPP
#define BFS_HPP

#include "graph/itree.hpp"
#include "graph/igraph.hpp"
#include "graph/adjecencylisttree.hpp"

#include <memory>
#include <queue>

namespace graph
{
    namespace algorithm
    {
        /*! Breadth-first search (BFS) is an algorithm for traversing or searching tree or graph data structures.
            It starts at the tree root (or some arbitrary node of a graph, sometimes referred to as a 'search key' and explores the neighbor nodes first, before moving to the next level neighbors.
            BFS was invented in the late 1950s by E. F. Moore, who used it to find the shortest path out of a maze,[2] and discovered independently by C. Y. Lee as a wire routing algorithm.
         */
        template <typename T>
        static std::unique_ptr<ITree<T>> bfs(const IGraph<T>& graph, const T& root)
        {
            auto tree  = new AdjecencyListTree<T>();

            // bfs build loop
            std::queue<T> q;
            q.push(root);
            tree->insertVertex(root);
            while(!q.empty())
            {
                auto &vertex = q.front();
                q.pop();

                if(tree->hasVertex(vertex) && !tree->outgoing(vertex).empty())
                {
                    continue;
                }

                for(auto &nextVertex : graph.outgoing(vertex))
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
                    q.push(nextVertex);

                }
            }

            return std::unique_ptr<ITree<T>>(tree);
        }
    }
}

#endif // BFS_HPP