#pragma once
#ifndef DEFAULTGRAPHRENDERER_HPP
#define DEFAULTGRAPHRENDERER_HPP

#include "graph/igraph.hpp"

#include "graph_ui/abstractgraphlayout.hpp"

#include "svg/svg.hpp"
#include "svg/circle.hpp"
#include "svg/rectangle.hpp"
#include "svg/line.hpp"

#include <memory>
#include <tuple>

namespace graphUI
{
    /*!
     */
    template <typename T>
    class DefaultGraphRenderer final
    {
        public:
            /*! constructor (default)
             */
            explicit DefaultGraphRenderer() = default;

            /*! destructor (default)
             */
            virtual ~DefaultGraphRenderer() = default;

            /*!
             */
            std::unique_ptr<SVG::SVG> render(const graph::IGraph<T> &g, const typename graphUI::AbstractGraphLayout<T>::LayoutType &layout, const typename graphUI::AbstractGraphLayout<T>::SizeFunctionType &size) const
            {
                // x and y margin
                auto xMargin = 50;
                auto yMargin = 50;

                // target width and height
                auto width = 0;
                auto height = 0;

                for(auto &vertex : g.vertices())
                {
                    auto vertexSize = size(vertex);
                    auto pos = std::get<0>(layout)(vertex);

                    auto x = std::get<0>(pos) + std::get<0>(vertexSize);
                    auto y = std::get<1>(pos) + std::get<1>(vertexSize);
                    width = x > width ? x : width;
                    height = y > height ? y : height;
                }
                width += xMargin;
                height += yMargin;

                // draw svg
                auto img = std::unique_ptr<SVG::SVG>(new SVG::SVG());
                img.get()->setWidth(width);
                img.get()->setHeight(height);

                // draw white background
                auto background = std::unique_ptr<SVG::Rectangle>(new SVG::Rectangle(0, 0, width, height));
                background.get()->setStrokeWidth(0);
                background.get()->setFillOpacity(1.0);
                background.get()->setFill(255,255,255);
                img.get()->add(std::move(background));

                // draw all edges
                for(auto &source : g.vertices())
                {
                    auto sourceVertexSize = size(source);
                    for(auto &target : g.outgoing(source))
                    {
                        auto targetVertexSize = size(target);
                        auto path = std::get<1>(layout)(source, target);
                        for(int i=1; i<path.size(); i++)
                        {
                            auto prevX = std::get<0>(path[i-1]) + std::get<0>(sourceVertexSize) / 2;
                            auto prevY = std::get<1>(path[i-1]) + std::get<1>(sourceVertexSize) / 2;
                            auto currX = std::get<0>(path[i]) + std::get<0>(targetVertexSize) / 2;
                            auto currY = std::get<1>(path[i]) + std::get<1>(targetVertexSize) / 2;
                            auto line = std::unique_ptr<SVG::Line>(new SVG::Line(prevX, prevY, currX, currY));
                            line.get()->setStroke(200,200,200);
                            img.get()->add(std::move(line));
                        }

                    }
                }

                // draw all vertices
                for(auto &vertex : g.vertices())
                {
                    auto vertexSize = size(vertex);
                    auto pos = std::get<0>(layout)(vertex);
                    auto x = std::get<0>(pos) + std::get<0>(vertexSize)/2;
                    auto y = std::get<1>(pos) + std::get<1>(vertexSize)/2;

                    // draw circle to represent vertex
                    auto vertexImg = std::unique_ptr<SVG::Circle>(new SVG::Circle(x, y, std::get<0>(vertexSize)/2));
                    vertexImg.get()->setStrokeWidth(1);
                    vertexImg.get()->setFillOpacity(1.0);
                    vertexImg.get()->setFill(43,172,43);
                    img.get()->add(std::move(vertexImg));
                }

                // return
                return img;
            }

        private:
    };
}

#endif // DEFAULTGRAPHRENDERER_HPP