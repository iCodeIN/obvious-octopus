#pragma once
#ifndef BFS_HPP
#define BFS_HPP

#include "itree.hpp"
#include "igraph.hpp"
#include "adjecencylisttree.hpp"

#include <memory>
#include <queue>

namespace graph
{
    /*!
     */
    template <typename T>
    static std::unique_ptr<ITree<T>> bfs(const IGraph<T>& graph, const T& root)
    {
        auto tree  = new AdjecencyListTree<T>();

        // bfs build loop
        std::queue<T> q;
        q.push(root);
        while(!q.empty())
        {
            auto &vertex = q.front();
            q.pop();

            if(!tree->outgoing(vertex).empty())
            {
                continue;
            }

            for(auto &nextVertex : graph.outgoing(vertex))
            {
                if(tree->incoming(nextVertex).empty())
                {
                    tree->insertEdge(vertex, nextVertex);
                    q.push(nextVertex);
                }
            }
        }

        return std::unique_ptr<ITree<T>>(tree);
    }
}

#endif // BFS_HPP