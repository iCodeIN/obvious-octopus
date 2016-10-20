#include "areaplot.h"

#include "svg/hsv.hpp"
#include "svg/svg.hpp"
#include "svg/rectangle.hpp"
#include "svg/polygon.hpp"

#include <algorithm>
#include <assert.h>
#include <iostream>

using namespace SVG;
using namespace plotting;

AreaPlot::AreaPlot(int w, int h)
    : m_width(w)
    , m_height(h)
{
    assert(w >= 0 && h >= 0);
}

void AreaPlot::addSeries(const AreaPlot::SeriesType& s)
{
    for(auto pair : s)
    {
        auto y = pair.second;
        assert(y >= 0);
    }
    m_series.push_back(std::cref(s));
}

std::unique_ptr<SVG::SVG> AreaPlot::render()
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
