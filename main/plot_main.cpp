#include <iostream>
#include <math.h>
#include <utility>
#include <vector>

#include "matrix/boostmatriximpl.hpp"
#include "matrix/imatrix.hpp"

#include "svg/svg.hpp"
#include "plotting/areaplot.hpp"
#include "plotting/heatplot.hpp"
#include "plotting/lineplot.hpp"

using namespace matrix;
using namespace plotting;
using namespace std;

int make_heatplot()
{
    // random matrix
    auto m = BoostMatrixImpl::rand(10,10);

    // getter function for values
    std::function<double(int,int)> value = [&](int i, int j)
    {
        return m->get(i, j);
    };

    // heatplot
    auto plot = std::unique_ptr<HeatPlot>(new HeatPlot(400,400));
    plot->addMatrix(value, 10, 10);

    // svg representation
    auto svg = plot->render();

    // output to file
    ofstream myfile;
    myfile.open ("heatplot.svg");
    myfile << *svg;
    myfile.close();

    return 0;
}

int make_lineplot()
{
    // create series
    vector<pair<double,double>> ys0;
    vector<pair<double,double>> ys1;

    for(int i=0; i<1000; i++)
    {
        auto x = (double) i / 256.0;
        auto y0 = abs(sin(x)) * 400;
        auto y1 = abs(cos(x)) * 400;
        ys0.push_back(make_pair(x, y0));
        ys1.push_back(make_pair(x, y1));
    }

    // lineplot
    auto plot = std::unique_ptr<LinePlot>(new LinePlot(400, 400));
    plot->addSeries(ys0);
    plot->addSeries(ys1);

    // svg representation
    auto svg = plot->render();

    // output to file
    ofstream myfile;
    myfile.open ("lineplot.svg");
    myfile << *svg;
    myfile.close();

    // return
    return 0;
}

int make_areaplot()
{
    // create series
    vector<pair<double,double>> ys0;
    vector<pair<double,double>> ys1;

    for(int i=0; i<1000; i++)
    {
        auto x = (double) i / 256.0;
        auto y0 = abs(sin(x * 5)) * 400;
        auto y1 = abs(cos(x)) * 400;
        ys0.push_back(make_pair(x, y0));
        ys1.push_back(make_pair(x, y1));
    }

    // lineplot
    auto plot = std::unique_ptr<AreaPlot>(new AreaPlot(400, 400));
    plot->addSeries(ys0);
    plot->addSeries(ys1);

    // svg representation
    auto svg = plot->render();

    // output to file
    ofstream myfile;
    myfile.open ("areaplot.svg");
    myfile << *svg;
    myfile.close();

    // return
    return 0;
}

int main()
{
    // heatplot
    make_heatplot();

    // lineplot
    make_lineplot();

    // areaplot
    make_areaplot();

    // return
    return 0;
}
