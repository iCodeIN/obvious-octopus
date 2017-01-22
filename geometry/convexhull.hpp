#ifndef CONVEXHULL_HPP
#define CONVEXHULL_HPP

namespace geometry
{
    /*! In mathematics, the convex hull or convex envelope of a set X of points in the Euclidean plane or in a Euclidean space (or, more generally, in an affine space over the reals)
        is the smallest convex set that contains X. For instance, when X is a bounded subset of the plane, the convex hull may be visualized as the shape enclosed by a rubber band stretched around X.
        Formally, the convex hull may be defined as the intersection of all convex sets containing X or as the set of all convex combinations of points in X.
        With the latter definition, convex hulls may be extended from Euclidean spaces to arbitrary real vector spaces; they may also be generalized further, to oriented matroids.
        The algorithmic problem of finding the convex hull of a finite set of points in the plane or other low-dimensional Euclidean spaces is one of the fundamental problems of computational geometry.
     */
    class ConvexHull
    {
        public:

            /*! Prohibit construction of ConvexHull. This class offers only static methods.
             */
            explicit ConvexHull() = delete;

            /*! define 2-dimensional point
             */
            using PointType = std::tuple<double,double>;

            /*! \return the convex hull of a given cloud of points
             */
            static std::vector<PointType> wrap(const std::vector<PointType>& points)
            {
                // find corner points
                auto corner = points[0];
                for(auto &point : points)
                {
                    auto d0 = std::get<0>(point) * std::get<0>(point) + std::get<1>(point) * std::get<1>(point);
                    auto d1 = std::get<0>(corner) * std::get<0>(corner) + std::get<1>(corner) * std::get<1>(corner);
                    if(d0 < d1)
                    {
                        corner = point;
                    }
                }

                // init wrap
                std::vector<PointType> retval;
                retval.push_back(corner);

                // main loop
                while(retval.size() == 1 && retval[0] != retval[retval.size() -1 ])
                {
                    auto curr = retval[retval.size() - 1];
                    auto prev = retval.size() == 1 ? std::make_tuple(std::get<0>(curr), 0) : retval[retval.size() - 2];

                    // find next point
                    auto candidate = points[0];
                    for(auto &point : points)
                    {
                        if(angle(prev, curr, point) > angle(prev, curr, candidate))
                        {
                            candidate = point;
                        }
                    }
                    retval.push_back(candidate);
                }

                // return
                return retval;
            }
        private:
            // --- methods ---
            double angle(const PointType& p0, const PointType& p1, const PointType &p2)
            {
                return 0.0;
            }

            // --- members ---
    };
}

#endif // CONVEXHULL_HPP