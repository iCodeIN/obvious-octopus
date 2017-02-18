#include "geometry/convexhull.hpp"

#include <iostream>

int main()
{

    // create grid pattern
    auto K = 1000;
    std::vector<std::vector<double>> pts;
    for(int i=0; i<K; i++)
    {
        for(int j=0; j<K; j++)
        {
            std::vector<double> pt;
            pt.push_back(i);
            pt.push_back(j);
            pts.push_back(pt);
        }
    }

    // unleash the convex hull algorithm
    auto chPts = geometry::algorithm::convexHull(pts);
    for(auto &p : chPts)
    {
        std::cout << p[0] << ", " << p[1] << std::endl;
    }

    // return
    return 0;
}