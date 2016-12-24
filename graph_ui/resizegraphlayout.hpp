#pragma once
#ifndef RESIZE_GRAPH_LAYOUT_HPP
#define RESIZE_GRAPH_LAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"

#include "graph/i2dgraph.hpp"
#include "graph/igraph.hpp"

#include <memory>

namespace graph_ui
{
    template <typename T>
    class ResizeGraphLayout : public AbstractGraphLayout<T>
    {
        public:
            /*! default constructor
             */
            public ResizeGraphLayout(std::unique_ptr<IGraphLayout<T>> &wrappedLayout, int width, int height)
                : m_wrappedLayout(wrappedLayout)
                , m_maxWidth(width)
                , m_maxHeight(height)
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
                }

                // adjust coordinates of edges
                for(auto &source : graph.vertices())
                {
                    for(auto &target : graph.vertices())
                    {
                        if(graph.hasVertex(source, target))
                        {
                            auto &path = initLayout->getEdgePoints(source, target);
                        }
                    }
                }

                // return
                return std::move(initLayout);
            }

        private:
            // --- methods ---
            // --- members ---
            int                                 m_maxWidth;
            int                                 m_maxHeight;
            std::unique_ptr<IGraphLayout<T>>    m_wrappedLayout;
    };
}

#endif // RESIZE_GRAPH_LAYOUT.HPP