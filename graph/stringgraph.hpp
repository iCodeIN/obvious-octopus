#ifndef STRINGGRAPH_HPP
#define STRINGGRAPH_HPP

#include "graph/adjecencylistgraph.hpp"

#include <memory>
#include <string>

namespace graph
{
    namespace algorithm
    {
        template <typename T>
        std::unique_ptr<IGraph<std::string>> stringify(const IGraph<T>& graph, std::function<std::string(const T& t)> toStringFunction)
        {
            auto retval = std::unique_ptr<IGraph<std::string>>(new AdjecencyListGraph<std::string>());

            for(auto &vertex : graph.vertices())
            {
                retval->insertVertex(toStringFunction(vertex));
            }

            for(auto &v0 : graph.vertices())
            {
                for(auto &v1 : graph.vertices())
                {
                    if(graph.hasEdge(v0, v1))
                    {
                        retval->insertEdge(toStringFunction(v0), toStringFunction(v1));
                    }
                }
            }

            return std::move(retval);
        }
    }
}

#endif // STRINGGRAPH_HPP