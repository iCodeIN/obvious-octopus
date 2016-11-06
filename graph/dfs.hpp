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
        while(!s.empty())
        {
            auto &vertex = s.top();
            s.pop();

            if(!tree->outgoing(vertex).empty())
            {
                continue;
            }

            for(auto &nextVertex : graph.outgoing(vertex))
            {
                if(tree->incoming(nextVertex).empty())
                {
                    tree->insertEdge(vertex, nextVertex);
                    s.push(nextVertex);
                }
            }
        }

        return std::unique_ptr<ITree<T>>(tree);
    }
}

#endif // DFS_HPP