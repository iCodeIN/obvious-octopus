#ifndef EIGENVALUES_HPP
#define EIGENVALUES_HPP

#include "graph/igraph.hpp"

#include <map>

namespace graph
{
    namespace algorithm
    {
        template <typename T>
        static std::map<T, double> eigenvalues(const IGraph<T> &graph)
        {
            // init
            std::map<T,double> ev0;
            std::map<T,double> ev1;
            auto n = graph.vertices().size();
            auto alpha = 0.15;
            for(auto &vertex : graph.vertices())
            {
                ev0[vertex] = 1.0;
                ev1[vertex] = 0.0;
            }

            // iterate
            auto diff = 1.0;
            while(diff > 0.000001)
            {
                for(auto &src : graph.vertices())
                {
                    auto out = graph.outgoing(src);
                    for(auto &dst : out)
                    {
                        ev1[dst] = ev1[dst] + (ev0[src] / out.size());
                    }
                }
                ev0.clear();
                diff = 0.0;
                for(auto &vertex : graph.vertices())
                {
                    ev0[vertex] = (alpha / n) + (1.0 - alpha) * ev1[vertex];
                    diff = std::max(ev0[vertex] - ev1[vertex], diff);
                    ev1[vertex] = 0.0;
                }
            }

            // return
            return ev0;
        }
    }
}

#endif // EIGENVALUES_HPP