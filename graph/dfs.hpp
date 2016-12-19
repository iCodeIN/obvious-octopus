#pragma once
#ifndef DFS_HPP
#define DFS_HPP

#include "itree.hpp"
#include "igraph.hpp"
#include "adjecencylisttree.hpp"

#include <memory>
#include <stack>

namespace graph
{
    /*!
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
            auto &vertex = s.top();
            s.pop();

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
                s.push(nextVertex);
            }
        }

        return std::unique_ptr<ITree<T>>(tree);
    }
}

#endif // DFS_HPP