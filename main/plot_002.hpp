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
        auto x = i / 10.0;
        auto y0 = 0.5 * i * i;
        auto y1 = (0.5 * x * x) +(5 * x);
        s0.push_back(std::pair<double,double>(x, y0));
        s1.push_back(std::pair<double,double>(x, y1));
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