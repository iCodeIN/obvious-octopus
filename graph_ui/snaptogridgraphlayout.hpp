#pragma once
#ifndef SNAPTOGRIDGRAPHLAYOUT_HPP
#define SNAPTOGRIDGRAPHLAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"

#include <math.h>
#include <memory>
#include <tuple>

namespace graphUI
{
    /*! Wrapper class that snaps all layed out components of its wrapped layout to a grid with given size.
     */
    template <typename T>
    class SnapToGridGraphLayout final : public AbstractGraphLayout<T>
    {
        public:
            /* constructor
                \param[in] innerLayout  the layout to be wrapped
             */
            explicit SnapToGridGraphLayout(std::unique_ptr<AbstractGraphLayout<T>> innerLayout)
                : m_innerLayout(std::move(innerLayout))
            {
            }

            /*! destructor (default)
             */
            virtual ~SnapToGridGraphLayout() = default;

            // --- AbstractGraphLayout ---
            virtual void setMinXMargin(int margin) override
            {
                m_innerLayout.get()->setMinXMargin(margin);
            }
            // --- AbstractGraphLayout ---
            virtual int getMinXMargin() const override
            {
                return m_innerLayout.get()->getMinXMargin();
            }
            // --- AbstractGraphLayout ---
            virtual void setMaxXMargin(int margin) override
            {
                m_innerLayout.get()->setMaxXMargin(margin);
            }
            // --- AbstractGraphLayout ---
            virtual int getMaxXMargin() const override
            {
                return m_innerLayout.get()->getMaxXMargin();
            }
            // --- AbstractGraphLayout ---
            virtual void setMinYMargin(int margin) override
            {
                m_innerLayout.get()->setMinYMargin(margin);
            }
            // --- AbstractGraphLayout ---
            virtual int getMinYMargin() const override
            {
                return m_innerLayout.get()->getMinYMargin();
            }
            // --- AbstractGraphLayout ---
            virtual void setMaxYMargin(int margin) override
            {
                m_innerLayout.get()->setMaxYMargin(margin);
            }
            // --- AbstractGraphLayout ---
            virtual int getMaxYMargin() const override
            {
                return m_innerLayout.get()->getMaxYMargin();
            }

            // --- AbstractGraphLayout ---
            virtual typename graphUI::AbstractGraphLayout<T>::LayoutType layout(const graph::IGraph<T> &graph, const typename graphUI::AbstractGraphLayout<T>::SizeFunctionType &size) override
            {
                auto initialLayout = m_innerLayout.get()->layout(graph, size);
                auto &vertexLayout = std::get<0>(initialLayout);
                auto &edgeLayout = std::get<1>(initialLayout);

                // snap vertices to grid
                auto gridVertexLayout = [vertexLayout, this](const T &vertex)
                {
                    auto pos = vertexLayout(vertex);
                    auto x = std::get<0>(pos);
                    auto y = std::get<1>(pos);
                    auto gridX = round(x / (double) m_xGridSize) * m_xGridSize;
                    auto gridY = round(y / (double) m_yGridSize) * m_yGridSize;
                    return std::make_tuple(gridX, gridY);
                };

                // snap edges to grid
                auto gridEdgeLayout = [edgeLayout, this](const T &source, const T &target)
                {
                    using PointType = typename graphUI::AbstractGraphLayout<T>::PointType;
                    std::vector<PointType> pos = edgeLayout(source, target);
                    for(int i=0; i<pos.size(); i++)
                    {
                        auto x = std::get<0>(pos[i]);
                        auto y = std::get<1>(pos[i]);
                        auto gridX = round(x / (double) m_xGridSize) * m_xGridSize;
                        auto gridY = round(y / (double) m_yGridSize) * m_yGridSize;
                        pos[i] = std::make_tuple(gridX, gridY);
                    }
                    return pos;
                };

                // return
                return std::make_tuple(gridVertexLayout, gridEdgeLayout);
            }

        private:
            std::unique_ptr<AbstractGraphLayout<T>> m_innerLayout;
            int                                     m_xGridSize = 10;
            int                                     m_yGridSize = 10;
    };
}

#endif // SNAPTOGRIDGRAPHLAYOUT_HPP