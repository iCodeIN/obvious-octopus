#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <algorithm>
#include <assert.h>
#include <functional>
#include <map>
#include <random>
#include <vector>

namespace geometry
{
    /*! k-means clustering is a method of vector quantization, originally from signal processing, that is popular for cluster analysis in data mining.
        k-means clustering aims to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean, serving as a prototype of the cluster.
        This results in a partitioning of the data space into Voronoi cells.
        The problem is computationally difficult (NP-hard); however, there are efficient heuristic algorithms that are commonly employed and converge quickly to a local optimum.
        These are usually similar to the expectation-maximization algorithm for mixtures of Gaussian distributions via an iterative refinement approach employed by both algorithms.
        Additionally, they both use cluster centers to model the data; however, k-means clustering tends to find clusters of comparable spatial extent,
        while the expectation-maximization mechanism allows clusters to have different shapes.
     */
    class KMeans
    {
        public:

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
                assert(MAX_ITERATIONS > 0);

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

                    if(iterationNr >= MAX_ITERATIONS)
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

        private:
            // --- methods ---
            static PointType zero(int d)
            {
                std::vector<double> v;
                for(int i=0; i<d; i++)
                {
                    v.push_back(0.0);
                }
                return v;
            }
            static PointType scalar(const PointType& p, double s)
            {
                std::vector<double> v;
                for(int i=0; i<p.size(); i++)
                {
                    v.push_back(p[i] * s);
                }
                return v;
            }
            // --- members ---
            static const int MAX_ITERATIONS = 1024;
    };
}

#endif // KMEANS_HPP