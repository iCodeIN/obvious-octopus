#pragma once
#ifndef GEOMETRY_CONVEXHULL_HPP
#define GEOMETRY_CONVEXHULL_HPP

#include <algorithm>
#include <cmath>
#include <math.h>
#include <vector>

namespace
{
    double polarAngle(   const std::vector<double>& A,
                         const std::vector<double>& B)
    {
        auto x = B[0] - A[0];
        auto y = B[1] - A[1];
        if(x == 0.0 && y == 0.0)
        {
            return  0.0;
        }
        auto r = std::atan2(y, x);
        if(r == 0)
        {
            return 180;
        }
        return (r * 180.0) / 3.141592654;
    }

    double angle(const std::vector<double>& A,
                 const std::vector<double>& B,
                 const std::vector<double>& C)
    {
        auto a = std::sqrt((B[0] - C[0])*(B[0] - C[0]) + (B[1] - C[1])*(B[1] - C[1]));
        auto b = std::sqrt((A[0] - C[0])*(A[0] - C[0]) + (A[1] - C[1])*(A[1] - C[1]));
        auto c = std::sqrt((B[0] - A[0])*(B[0] - A[0]) + (B[1] - A[1])*(B[1] - A[1]));
        if(a==0.0 || b==0.0 || c==0.0)
        {
            return 0.0;
        }
        auto r = std::acos((c*c + a*a - b*b) / (2*c*a));
        return std::floor(r * 10000)/10000;
    }

    double dist(   const std::vector<double>& A,
                   const std::vector<double>& B)
    {
        auto xD = A[0] - B[0];
        auto yD = A[1] - B[1];
        return std::sqrt(xD*xD + yD*yD);
    }
}

namespace geometry
{
    namespace algorithm
    {
        /*! define 2-dimensional point
         */
        using PointType = std::vector<double>;

        /*! In mathematics, the convex hull or convex envelope of a set X of points in the Euclidean plane or in a Euclidean space (or, more generally, in an affine space over the reals)
            is the smallest convex set that contains X. For instance, when X is a bounded subset of the plane, the convex hull may be visualized as the shape enclosed by a rubber band stretched around X.
            Formally, the convex hull may be defined as the intersection of all convex sets containing X or as the set of all convex combinations of points in X.
            With the latter definition, convex hulls may be extended from Euclidean spaces to arbitrary real vector spaces; they may also be generalized further, to oriented matroids.
            The algorithmic problem of finding the convex hull of a finite set of points in the plane or other low-dimensional Euclidean spaces is one of the fundamental problems of computational geometry.
        */
        static std::vector<PointType> convexHull(const std::vector<PointType>& points)
        {
            // set up return value
            std::vector<PointType> hull;

            // if less than 3 points
            if(points.size() <= 2)
            {
                hull.push_back(points[0]);
                hull.push_back(points[1]);
                return hull;
            }

            // find extreme point
            auto tmp = points[0];
            for(int i=0; i<points.size(); i++)
            {
                auto &point = points[i];
                if(point[0] < tmp[0] && point[1] < tmp[1])
                {
                    tmp = point;
                }
            }
            hull.push_back(tmp);

            // find point that maximizes polar angle
            tmp = hull[0];
            for(auto &point : points)
            {
                if(polarAngle(hull[0], point) > polarAngle(hull[0], tmp))
                {
                    tmp = point;
                }
                else if(polarAngle(hull[0], point) == polarAngle(hull[0], tmp) && dist(hull[0], point) > dist(hull[0], tmp))
                {
                    tmp = point;
                }
            }
            hull.push_back(tmp);

            // main loop
            while(hull[0] != hull[hull.size() - 1])
            {
                auto &A = hull[hull.size() - 2];
                auto &B = hull[hull.size() - 1];
                auto C = points[0];
                auto maxAngle = angle(A,B,C);
                for(auto &point : points)
                {
                    auto tmpAngle = angle(A, B, point);
                    if(tmpAngle > maxAngle)
                    {
                        maxAngle = tmpAngle;
                        C = point;
                    }
                    if(tmpAngle == maxAngle && dist(B, point) > dist(B, C))
                    {
                        maxAngle = tmpAngle;
                        C = point;
                    }
                }
                hull.push_back(C);
            }
            hull.erase(hull.begin() + hull.size());

            // return
            return hull;

        }

    }
}

#endif // GEOMETRY_CONVEXHULL_HPP