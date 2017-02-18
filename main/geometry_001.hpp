#include "geometry/convexhull.hpp"
#include "geometry/kmeans.hpp"

#include <iostream>

int main()
{

    // create grid pattern
    auto K = 10;
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

    auto clusters =  geometry::algorithm::kmeansWithEuclideanMetric(pts, 4);

    // extract cluster 0
    std::vector<std::vector<double>> cluster0;
    for(auto &p : clusters)
    {
        if(p.second == 0)
        {
            cluster0.push_back(p.first);
        }
    }
    std::cout << "cluster : " << std::endl;
    for(auto &p : cluster0)
    {
        std::cout << p[0] << ", " << p[1] << std::endl;
    }

    // unleash the convex hull algorithm
    auto clusterHull = geometry::algorithm::convexHull(cluster0);

    std::cout << "hull : " << std::endl;
    for(auto &p : clusterHull)
    {
        std::cout << p[0] << ", " << p[1] << std::endl;
    }

    // return
    return 0;
}