#pragma once
#ifndef RANDOM_GRAPH_LAYOUT_HPP
#define RANDOM_GRAPH_LAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"

#include "graph/adjecencylistgraph.hpp"
#include "graph/i2dgraph.hpp"
#include "graph/igraph.hpp"

#include <memory>
#include <random>

namespace graph_ui
{
    /*! Random graph layout
     */
    template <typename T>
    class RandomGraphLayout : public AbstractGraphLayout<T>
    {
        public:
            /*! default constructor
             */
            explicit RandomGraphLayout() = default;

            /*! default destructor
             */
            virtual ~RandomGraphLayout() = default;

            //! --- AbstractGraphLayout ---
            virtual std::unique_ptr<graph::I2DGraph<T>> layout(const graph::IGraph<T> &graph, const typename AbstractGraphLayout<T>::SizeFunctionType &size) const override
            {
                // create new I2DGraph
                auto retval = std::unique_ptr<graph::AdjecencyListGraph<T>>(new graph::AdjecencyListGraph<T>());

                // init random
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<ulong> dis(0, 1000);

                // add vertices
                for(auto &vertex : graph.vertices())
                {
                    retval->insertVertex(vertex);
                    // assign random coordinates to each vertex
                    auto x = dis(gen);
                    auto y = dis(gen);
                    retval->setVertexPoint(vertex, std::make_pair(x,y));
                }

                // add vertices
                for(auto &source : graph.vertices())
                {
                    for(auto &target : graph.vertices())
                    {
                        if(graph.hasEdge(source, target))
                        {
                            retval->insertEdge(source, target);
                            // apply straight line for each edge
                            std::vector<std::pair<int,int>> edgePoints;
                            edgePoints.push_back(retval->getVertexPoint(source));
                            edgePoints.push_back(retval->getVertexPoint(target));
                            retval->setEdgePoints(source, target, edgePoints);
                        }
                    }
                }

                // return
                return std::move(retval);
            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // RANDOM_GRAPH_LAYOUT_HPP