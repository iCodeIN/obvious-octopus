#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <algorithm>
#include <assert.h>
#include <functional>
#include <map>
#include <random>
#include <vector>

namespace
{
    static std::vector<double> zero(int d)
    {
        std::vector<double> v;
        for(int i=0; i<d; i++)
        {
            v.push_back(0.0);
        }
        return v;
    }
    static std::vector<double> scalar(const std::vector<double>& p, double s)
    {
        std::vector<double> v;
        for(int i=0; i<p.size(); i++)
        {
            v.push_back(p[i] * s);
        }
        return v;
    }
}

namespace geometry
{
    namespace algorithm
    {
        /*! define an n-dimensional point
         */
        using PointType = std::vector<double>;

        /*! define a matric function for n-dimensional points
         */
        using MetricFunctionType = std::function<double(const PointType& p0, const PointType& p1)>;

        /*! Apply k-means clustering to the given collection of points, with the given metric
            \param[in] points   the points to cluster
            \param[in] metric   the metric to use when measuring distance
            \param[in] k        the desired number of clusters
         */
        static std::map<PointType, int> kmeans(const std::vector<PointType>& points, const MetricFunctionType& metric, int k)
        {
            assert(!points.empty());
            assert(std::find_if(points.begin(), points.end(), [&points](const PointType& p)
            {
                return p.size() != points[0].size();
            }) == points.end());
            assert(k > 0);

            // determine dimensionality
            auto dim = points[0].size();

            // random numbers
            std::random_device randomDevice;
            std::default_random_engine randomEngine(randomDevice());
            std::uniform_int_distribution<int> uniform_dist(0, k-1);

            // initial (random) assignment
            std::map<PointType, int> clusters;
            for(auto &point : points)
            {
                clusters[point] = uniform_dist(randomEngine);
            }

            std::map<int, PointType> centroids;
            std::vector<PointType> previousCentroids;
            auto iterationNr = 0;
            while(true)
            {
                // find centroids
                centroids.clear();
                std::map<int, int> pointsPerCluster;
                for(int i=0; i<k; i++)
                {
                    centroids[i] = zero(dim);
                    pointsPerCluster[i] = 0;
                }
                for(auto &pair : clusters)
                {
                    for(int i=0; i<dim; i++)
                    {
                        centroids[pair.second][i] += pair.first[i];
                    }
                    pointsPerCluster[pair.second]++;
                }
                for(int i=0; i<k; i++)
                {
                    centroids[i] = scalar(centroids[i], 1.0 / pointsPerCluster[i]);
                }

                // assign each point to the cluster corresponding to the centroid
                clusters.clear();
                for(auto &point : points)
                {
                    auto optClusterIndex = 0;
                    for(auto &pair : centroids)
                    {
                        if(metric(pair.second, point) < metric(centroids[optClusterIndex], point))
                        {
                            optClusterIndex = pair.first;
                        }
                    }
                    clusters[point] = optClusterIndex;
                }

                // check stopping criteria
                std::vector<PointType> centroidsVector;
                for(auto &pair : centroids)
                {
                    centroidsVector.push_back(pair.second);
                }
                std::sort(centroidsVector.begin(), centroidsVector.end());
                if(previousCentroids == centroidsVector)
                {
                    break;
                }

                if(iterationNr >= 1024)
                {
                    break;
                }

                previousCentroids = centroidsVector;
            }

            // return
            return clusters;
        }

        /*! Apply k-means clustering to the given collection of points, with a Euclidean metric
            \param[in] points   the points to cluster
            \param[in] k        the desired number of clusters
         */
        static std::map<PointType, int> kmeansWithEuclideanMetric(const std::vector<PointType>& points, int k)
        {
            // use euclidean metric
            auto f = [](const PointType& p0, const PointType& p1)
            {
                auto dist = 0.0;
                for(int i=0; i<p0.size(); i++)
                {
                    dist += (p0[i] - p1[i]) * (p0[i] - p1[i]);
                }
                return dist;
            };

            // redirect
            return kmeans(points, f, k);
        }
    }
}

#endif // KMEANS_HPP