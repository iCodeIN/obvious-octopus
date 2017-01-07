#ifndef LINEPLOT_HPP
#define LINEPLOT_HPP

#include "svg/hsv.hpp"
#include "svg/line.hpp"
#include "svg/polyline.hpp"
#include "svg/rectangle.hpp"
#include "svg/svg.hpp"

#include <assert.h>
#include <memory>
#include <vector>

namespace plotting
{
    class LinePlot
    {
        public:
            /*! constructor
                \param[in] width    the desired width
                \param[in] height   the desired height
             */
            explicit LinePlot(int width, int height)
                : m_height(height)
                , m_width(width)
                , m_verticalAxis(true)
                , m_horizontalAxis(true)
                , m_verticalGrid(true)
                , m_horizontalGrid(true)
            {
                assert(m_height > 0 && m_width > 0);
            }

            /*! (empty) constructor with sensible defaults
             */
            explicit LinePlot()
                : m_height(400)
                , m_width(650)
                , m_verticalAxis(true)
                , m_horizontalAxis(true)
                , m_verticalGrid(true)
                , m_horizontalGrid(true)
            {
            }

            /*! destructor
             */
            virtual ~LinePlot() = default;

            /*! Prohibit copy constructor
             */
            LinePlot(LinePlot &n) = delete;

            /*! Prohibit const copy constructor
             */
            LinePlot(const LinePlot &n) = delete;

            /*! Prohibit assignment operator
             */
            LinePlot operator=(LinePlot &&) = delete;

            /*! Prohibit const assignment operator
             */
            LinePlot operator=(const LinePlot &&) = delete;

            /*! Define a Series
             */
            using SeriesType = std::vector<std::pair<double,double>>;

            /*! Add a series of x/y-values to this LinePlot
             */
            void addSeries(const SeriesType& s)
            {
                m_series.push_back(s);
            }

            // --- IPlot ---
            std::unique_ptr<SVG::SVG> plot() const
            {
                // create image
                auto img = std::unique_ptr<SVG::SVG>(new SVG::SVG());

                // set data range
                const_cast<LinePlot*>(this)->setDataRange();

                // draw background
                drawBackground(*img);

                // draw axis
                drawAxis(*img);

                // draw axis labels
                drawAxisLabels(*img);

                // draw grid
                drawGrid(*img);

                // draw series
                drawSeries(*img);

                // return
                return std::move(img);
            }

        private:
            // --- methods ---

            void setDataRange()
            {
                m_minX = m_series.empty() ? 0 : m_series[0][0].first;
                m_minY = m_series.empty() ? 0 : m_series[0][0].second;
                m_maxX = m_minX;
                m_maxY = m_minY;
                for(auto &serie : m_series)
                {
                    for(auto &point : serie)
                    {
                        auto x = point.first;
                        auto y = point.second;
                        m_minX = std::min(m_minX, x);
                        m_minY = std::min(m_minY, y);
                        m_maxX = std::max(m_maxX, x);
                        m_maxY = std::max(m_maxY, y);
                    }
                }
                m_xDataRange = m_maxX - m_minX;
                m_yDataRange = m_maxY - m_minY;
            }

            void drawBackground(SVG::SVG& svg) const
            {
                auto background = std::unique_ptr<SVG::Rectangle>(new SVG::Rectangle(0, 0, m_width, m_height));
                background->setStrokeWidth(0);
                background->setFillOpacity(1.0);
                background->setFill(255,255,255);
                svg.add(std::move(background));
            }

            void drawAxis(SVG::SVG& svg) const
            {
                if(m_horizontalAxis)
                {
                    auto line = std::unique_ptr<SVG::Line>(new SVG::Line(m_leftMargin, m_height - m_bottomMargin, m_width - m_rightMargin, m_height - m_bottomMargin));
                    line->setStrokeWidth(1);
                    line->setStroke(0, 0, 0);
                    svg.add(std::move(line));
                }
                if(m_verticalAxis)
                {
                    auto line = std::unique_ptr<SVG::Line>(new SVG::Line(m_leftMargin, m_height - m_bottomMargin, m_leftMargin, m_topMargin));
                    line->setStrokeWidth(1);
                    line->setStroke(0, 0, 0);
                    svg.add(std::move(line));
                }
            }

            void drawAxisLabels(SVG::SVG& svg) const
            {
                if(m_horizontalAxis)
                {
                    for(int i=0; i<=10; i++)
                    {
                        // TODO
                    }
                }
                if(m_verticalAxis)
                {
                    for(int i=0; i<=10; i++)
                    {
                        // TODO
                    }
                }
            }

            void drawGrid(SVG::SVG& svg) const
            {
                std::vector<int> dashes;
                dashes.push_back(1);
                dashes.push_back(1);
                if(m_horizontalGrid)
                {
                    for(int i=0; i<=10; i++)
                    {
                        auto y = (m_height - m_topMargin - m_bottomMargin) / 10;
                        auto line = std::unique_ptr<SVG::Line>(new SVG::Line(m_leftMargin, m_bottomMargin + y * i, m_width - m_rightMargin, m_bottomMargin + y * i));
                        line->setStrokeWidth(1);
                        line->setStroke(66,66,66);
                        line->setStrokeArray(dashes);
                        svg.add(std::move(line));
                    }
                }
                if(m_verticalGrid)
                {
                    for(int i=0; i<=10; i++)
                    {
                        auto x = (m_width - m_leftMargin - m_rightMargin) / 10;
                        auto line = std::unique_ptr<SVG::Line>(new SVG::Line(m_leftMargin + x * i, m_topMargin, m_leftMargin + x * i, m_height - m_bottomMargin));
                        line->setStrokeWidth(1);
                        line->setStroke(66,66,66);
                        line->setStrokeArray(dashes);
                        svg.add(std::move(line));
                    }
                }
            }

            void drawSeries(SVG::SVG& svg) const
            {
                // calculate scaling information for all series
                auto xPlotRange = m_width - m_leftMargin - m_rightMargin;
                auto yPlotRange = m_height - m_topMargin - m_bottomMargin;

                // draw series
                auto colors = SVG::HSV::equidistant(m_series.size() + 2);
                for(int i=0; i<m_series.size(); i++)
                {
                    std::vector<std::pair<double,double>> modPoints;
                    for(auto &point : m_series[i])
                    {
                        auto x = point.first;
                        auto y = point.second;
                        modPoints.push_back(std::pair<double,double>((x - m_minX) * xPlotRange / m_xDataRange + m_leftMargin, m_height - m_bottomMargin - (y - m_minY) * yPlotRange / m_yDataRange));
                    }
                    auto line = std::unique_ptr<SVG::Polyline>(new SVG::Polyline(modPoints));
                    line->setStrokeWidth(1);
                    auto color = SVG::HSV::toRGB(colors[i]);
                    line->setStroke(std::get<0>(color), std::get<1>(color), std::get<2>(color));
                    svg.add(std::move(line));
                }
            }

            // --- members ---
            std::vector<SeriesType>    m_series;
            bool                       m_horizontalAxis;
            bool                       m_verticalAxis;

            bool                       m_horizontalGrid;
            bool                       m_verticalGrid;

            double                      m_minX = 0;
            double                      m_minY = 0;
            double                      m_maxX = 0;
            double                      m_maxY = 0;
            double                      m_xDataRange = 0;
            double                      m_yDataRange = 0;

            int                         m_leftMargin     = 10;
            int                         m_rightMargin    = 10;
            int                         m_topMargin      = 10;
            int                         m_bottomMargin   = 10;

            int                         m_width;
            int                         m_height;
    };
}

#endif // LINEPLOT_HPP