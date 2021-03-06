#pragma once
#ifndef GRAPH_UI_RESIZEGRAPHLAYOUT_HPP
#define GRAPH_UI_RESIZEGRAPHLAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"

#include "graph/i2dgraph.hpp"
#include "graph/igraph.hpp"

#include <memory>

namespace graph_ui
{
    /*! IGraphLayout implementation that resizes the output of another IGraphLayout to fit a given width/height
     */
    template <typename T>
    class ResizeGraphLayout : public AbstractGraphLayout<T>
    {
        public:
            /*! constructor
             */
            public ResizeGraphLayout(std::unique_ptr<IGraphLayout<T>> &wrappedLayout, int width, int height)
                : m_wrappedLayout(wrappedLayout)
                , m_maxWidth(width)
                , m_maxHeight(height)
            {
                assert(m_wrappedLayout);
            }

            /*! default destructor
             */
            virtual ~ResizeGraphLayout() = default;

            //! --- AbstractGraphLayout ---
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

#endif // GRAPH_UI_RESIZEGRAPHLAYOUT.HPP