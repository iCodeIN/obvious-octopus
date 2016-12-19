#pragma once
#ifndef SNAP_TO_GRID_GRAPH_LAYOUT_HPP
#define SNAP_TO_GRID_GRAPH_LAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"

#include "graph/i2dgraph.hpp"
#include "graph/igraph.hpp"

#include <memory>

namespace graph_ui
{
    template <typename T>
    class SnapToGridGraphLayout : public AbstractGraphLayout<T>
    {
        public:
            /*! default constructor
             */
            public SnapToGridGraphLayout(std::unique_ptr<IGraphLayout<T>> &wrappedLayout)
                : m_wrappedLayout(wrappedLayout)
                , m_xGridResolution(20)
                , m_yGridResolution(20)
            {
                assert(m_wrappedLayout);
            }

            // --- AbstractGraphLayout ---
            virtual std::unique_ptr<graph::I2DGraph<T>> layout(const graph::IGraph<T> &graph, const typename AbstractGraphLayout<T>::SizeFunctionType &size) const override
            {
                // perform initial layout
                auto initLayout = m_wrappedLayout->layout(graph, size);
                assert(initLayout);

                // adjust coordinates of vertices
                for(auto &vertex : graph.vertices())
                {
                    auto &point = initLayout->getVertexPoint(vertex);
                    point.first -= point.first % m_xGridResolution;
                    point.second -= point.second % m_yGridResolution;
                    initLayout->setVertexPoint(vertex, x, y);
                }

                // adjust coordinates of edges
                for(auto &source : graph.vertices())
                {
                    for(auto &target : graph.vertices())
                    {
                        if(graph.hasVertex(source, target))
                        {
                            auto &path = initLayout->getEdgePoints(source, target);
                            for(auto &point : path)
                            {
                                point.first -= point.first % m_xGridResolution;
                                point.second -= point.second % m_yGridResolution;
                            }
                        }
                    }
                }

                // return
                return std::move(initLayout);
            }

        private:
            // --- methods ---
            // --- members ---
            int                                 m_xGridResolution;
            int                                 m_yGridResolution;
            std::unique_ptr<IGraphLayout<T>>    m_wrappedLayout;
    };
}

#endif // SNAP_TO_GRID_GRAPH_LAYOUT_HPP