#pragma once
#ifndef HEATPLOT_HPP
#define HEATPLOT_HPP

#include "svg/hsv.hpp"
#include "svg/svg.hpp"
#include "svg/rectangle.hpp"

#include <assert.h>
#include <functional>
#include <memory>

namespace plotting
{
    /*! A heat map (or heatmap) is a graphical representation of data where the individual values contained in a matrix are represented as colors.
        The term Heatmap was originally coined and trademarked by software designer Cormac Kinney in 1991,
        to describe a 2D display depicting real time financial market information. Heat maps originated in 2D displays of the values in a data matrix.
        Larger values were represented by small dark gray or black squares (pixels) and smaller values by lighter squares.
     */
    class HeatPlot final
    {
        public:
            /*! Constructor
                \param[in] w width of the HeatPlot
                \param[in] h height of the HeatPlot
             */
            explicit HeatPlot(int w, int h)
                : m_width(w)
                , m_height(h)
            {
                assert(w >= 0 && h >= 0);
            }

            /*! Default destructor
             */
            virtual ~HeatPlot() = default;

            /*! Prohibit copy constructor
             */
            HeatPlot(HeatPlot &n) = delete;

            /*! Prohibit const copy constructor
             */
            HeatPlot(const HeatPlot &n) = delete;

            /*! Prohibit assignment operator
             */
            HeatPlot operator=(HeatPlot &&) = delete;

            /*! Prohibit const assignment operator
             */
            HeatPlot operator=(const HeatPlot &&) = delete;

            /*! Add a matrix to this HeatPlot
             */
            void addMatrix(std::function<double(int,int)> &valueGetter, int rows, int cols)
            {
                m_valueGetter = valueGetter;
                m_rows = rows;
                m_cols = cols;
            }

            /*! Render a HeatPlot as an SVG image
             */
            std::unique_ptr<SVG::SVG> render() const
            {
                // construct SVG
                auto img = std::unique_ptr<SVG::SVG>(new SVG::SVG());

                // find largest x and y value
                auto maxX = m_cols * 10;
                auto maxY = m_rows * 10;
                img.get()->setViewBox(0, 0, maxX, maxY);
                img.get()->setWidth(m_width);
                img.get()->setHeight(m_height);

                // add white background
                auto background = std::unique_ptr<SVG::Rectangle>(new SVG::Rectangle(0, 0, maxX, maxY));
                background.get()->setStrokeWidth(0);
                background.get()->setFillOpacity(1.0);
                background.get()->setFill(255,255,255);
                img.get()->add(std::move(background));

                // get minimum and maximum, to determine relative size of elements in the matrix
                auto max = m_valueGetter(0,0);
                auto min = m_valueGetter(0,0);
                for(int i=0; i<m_rows; i++)
                {
                    for(int j=0; j<m_cols; j++)
                    {
                        max = std::max(max, m_valueGetter(i,j));
                        min = std::min(min, m_valueGetter(i,j));
                    }
                }

                // add tiles
                for(int i=0; i<m_rows; i++)
                {
                    for(int j=0; j<m_cols; j++)
                    {
                        auto cell  = std::unique_ptr<SVG::Rectangle>(new SVG::Rectangle(i*10, j*10, 10, 10));
                        cell.get()->setStrokeWidth(0);
                        cell.get()->setFillOpacity(1.0);

                        double rVal = (m_valueGetter(i,j) + std::abs(min)) / (max + std::abs(min));
                        auto color = SVG::HSV::toRGB(std::make_tuple(120, rVal, 0.58));

                        cell.get()->setFill(std::get<0>(color), std::get<1>(color), std::get<2>(color));
                        img.get()->add(std::move(cell));
                    }
                }

                // return
                return img;
            }

        private:
            // --- Members ---
            std::function<double(int,int)>         m_valueGetter;
            int                                    m_rows;
            int                                    m_cols;

            int                                    m_width;
            int                                    m_height;
    };
}

#endif // HEATPLOT_HPP
