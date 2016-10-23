#pragma once
#ifndef AREAPLOT_HPP
#define AREAPLOT_HPP

#include "svg/hsv.hpp"
#include "svg/svg.hpp"
#include "svg/rectangle.hpp"
#include "svg/polygon.hpp"

#include <algorithm>
#include <assert.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace plotting
{
    /*! Area charts are used to represent cumulated totals using numbers or percentages (stacked area charts in this case) over time.
    	 Use the area chart for showing trends over time among related attributes.
    	 The area chart is like the plot chart except that the area below the plotted line is filled in with color to indicate volume.
    	 When multiple attributes are included, the first attribute is plotted as a line with color fill followed by the second attribute, and so on.
     */
    class AreaPlot final
    {
        public:
            /*! Constructor
            	 \param[in] w	width of the AreaPlot
            	 \param[in] h	height of the AreaPlot
             */
            explicit AreaPlot(int w, int h)
                : m_width(w)
                , m_height(h)
            {
                assert(w >= 0 && h >= 0);
            }

            /*! Default destructor
             */
            virtual ~AreaPlot() = default;

            /*! Prohibit copy constructor
             */
            AreaPlot(AreaPlot &n) = delete;

            /*! Prohibit const copy constructor
             */
            AreaPlot(const AreaPlot &n) = delete;

            /*! Prohibit assignment operator
             */
            AreaPlot operator=(AreaPlot &&) = delete;

            /*! Prohibit const assignment operator
             */
            AreaPlot operator=(const AreaPlot &&) = delete;

            /*! Define a Series
             */
            using SeriesType = std::vector<std::pair<double,double>>;

            /*! Add a series of x/y-values to this AreaPlot
             */
            void addSeries(const SeriesType& s)
            {
                for(auto pair : s)
                {
                    auto y = pair.second;
                    assert(y >= 0);
                }
                m_series.push_back(std::cref(s));
            }

            /*! Render a AreaPlot as an SVG image
             */
            std::unique_ptr<SVG::SVG> render()
            {
                // Construct SVG
                auto img = std::unique_ptr<SVG::SVG>(new SVG::SVG());

                // Find largest x and y value
                auto maxX = m_series[0].get()[0].first;
                auto maxY = m_series[0].get()[0].second;
                for(auto s : m_series)
                {
                    for(auto tuple : s.get())
                    {
                        maxX = std::max(maxX, tuple.first);
                        maxY = std::max(maxY, tuple.second);
                    }
                }
                img.get()->setViewBox(0, 0, maxX, maxY);
                img.get()->setWidth(m_width);
                img.get()->setHeight(m_height);

                // Add white background
                auto background = std::unique_ptr<Rectangle>(new Rectangle(0, 0, maxX, maxY));
                background.get()->setStrokeWidth(0);
                background.get()->setFillOpacity(1.0);
                background.get()->setFill(255,255,255);
                img.get()->add(std::move(background));

                // Add each series to the svg
                auto colors = HSV::equidistant(m_series.size());
                for(int i=0; i<m_series.size(); i++)
                {
                    auto s = m_series[i];
                    auto sRef = s.get();

                    // modify series to ensure closed shape
                    if(sRef[0].second != 0)
                    {
                        sRef.insert(sRef.begin(), std::make_pair(sRef[0].first, 0));
                    }
                    if(sRef[sRef.size()-1].second != 0)
                    {
                        sRef.push_back(std::make_pair(sRef[sRef.size()-1].first, 0));
                    }

                    // Set the color of each line
                    auto polyline = std::unique_ptr<Polygon>(new Polygon(sRef));
                    auto rgb = HSV::toRGB(colors[i]);
                    polyline.get()->setStroke(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
                    polyline.get()->setStrokeWidth(0.2);
                    polyline.get()->setStrokeOpacity(1.0);
                    polyline.get()->setFillOpacity(1.0);
                    polyline.get()->setFill(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
                    img.get()->add(std::move(polyline));
                }

                // flip image horizontally
                img.get()->flipHorizontal();

                // return
                return img;
            }

        private:
            // --- Members ---
            std::vector<std::reference_wrapper<const SeriesType>> 		m_series;      //!< All series to be included in this plot
            int                  													m_width;       //!< Output width
            int                  													m_height;      //!< Output height
    };
}

#endif // AREAPLOT_HPP
