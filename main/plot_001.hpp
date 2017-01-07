#ifndef PLOT_001_HPP
#define PLOT_001_HPP

#include "plotting/lineplot.hpp"

#include <iostream>

using namespace plotting;
using namespace std;


int main()
{
    // generate series
    std::vector<std::pair<double, double>> s0;
    std::vector<std::pair<double, double>> s1;
    for(int i=-100; i<=100; i++)
    {
        s0.push_back(std::pair<double,double>(i, sin(i / 10.0) ));
        s1.push_back(std::pair<double,double>(i, cos(i / 10.0) + sin(i/100.0) ));
    }

    // generate plot
    LinePlot lp;
    lp.addSeries(s0);
    lp.addSeries(s1);

    auto svgPtr = lp.plot();
    auto xmlPtr = svgPtr->toXML();

    cout << *xmlPtr;

    // return
    return 0;
}

#endif // PLOT_001_HPP