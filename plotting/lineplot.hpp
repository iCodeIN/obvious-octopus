#ifndef LINEPLOT_HPP
#define LINEPLOT_HPP

#include "svg/hsv.hpp"
#include "svg/svg.hpp"
#include "svg/rectangle.hpp"
#include "svg/polyline.hpp"

#include <algorithm>
#include <assert.h>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace plotting
{
    /*! A line chart or line graph is a type of chart which displays information as a series of data points called 'markers' connected by straight line segments.
    	 It is a basic type of chart common in many fields. It is similar to a scatter plot except that the measurement points are ordered (typically by their x-axis value)
    	 and joined with straight line segments. A line chart is often used to visualize a trend in data over intervals of time – a time series – thus the line is often drawn chronologically. In
    	 these cases they are also known as run charts.
     */
    class LinePlot final
    {
        public:
            /*! Constructor
            	 \param[in] w	width of the LinePlot
            	 \param[in] h	height of the LinePlot
             */
            explicit LinePlot(int w, int h)
                : m_width(w)
                , m_height(h)
            {
                assert(w >= 0 && h >= 0);
            }

            /*! Default destructor
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
                m_series.push_back(std::cref(s));
            }

            /*! Render a LinePlot as an SVG image
             */
            std::unique_ptr<SVG::SVG> render() const
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

                // Set viewbox, width and height
                img.get()->setViewBox(0, 0, maxX, maxY);
                img.get()->setWidth(m_width);
                img.get()->setHeight(m_height);

                // Add white background
                auto background = std::unique_ptr<SVG::Rectangle>(new SVG::Rectangle(0, 0, maxX, maxY));
                background.get()->setStrokeWidth(0);
                background.get()->setFillOpacity(1.0);
                background.get()->setFill(255,255,255);
                img.get()->add(std::move(background));

                // Add each series to the svg
                auto colors = SVG::HSV::equidistant(m_series.size());
                for(int i=0; i<m_series.size(); i++)
                {
                    auto s = m_series[i];

                    // Set the color of each line
                    auto polyline = std::unique_ptr<SVG::Polyline>(new SVG::Polyline(s));
                    auto rgb = SVG::HSV::toRGB(colors[i]);
                    polyline.get()->setStroke(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
                    polyline.get()->setStrokeWidth(0.2);
                    polyline.get()->setStrokeOpacity(1.0);
                    img.get()->add(std::move(polyline));
                }

                // Flip image horizontally
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

#endif // LINEPLOT_HPP
