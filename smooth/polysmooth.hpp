#pragma once
#ifndef POLYFIT_HPP
#define POLYFIT_HPP

#include "matrix/gaussianelimination.h"
#include "matrix/matrix.hpp"

#include <assert.h>
#include <math.h>
#include <vector>

namespace smooth
{
    /*! In statistics, polynomial regression is a form of linear regression in which the relationship between the independent
    	 variable x and the dependent variable y is modelled as an nth degree polynomial.
    	 Polynomial regression fits a nonlinear relationship between the value of x and the corresponding conditional mean of y,
    	 denoted E(y | x), and has been used to describe nonlinear phenomena such as the growth rate of tissues, the distribution of carbon isotopes
    	 in lake sediments, and the progression of disease epidemics. Although polynomial regression fits a nonlinear model to the data, as a statistical
    	 estimation problem it is linear, in the sense that the regression function E(y | x) is linear in the unknown parameters that are estimated from the data.
    	 For this reason, polynomial regression is considered to be a special case of multiple linear regression.
     */
    class PolynomialSmoothing final
    {
        public:
            /*! Prohibit construction of PolynomialSmoothing. This class offers only static methods.
             */
            explicit PolynomialSmoothing() = delete;

            /*! Define a Series
             */
            using SeriesType = std::vector<std::pair<double,double>>;

            /*! Fits a collection of points to a polynomial of given degree.
             	 This method uses a least-squares technique. It is not advised,
             	 both from the perspective of computational complexity,
             	 numeric stability, as well as for the sake of accuracy to use
             	 high-degree polynomials.
             	 Returns the coefficients of the polynomial from lowest degree
             	 to highest degree. Such that the coefficient at index 0
             	 corresponds to the coefficient for term 'x^0'.
             */
            static std::vector<double> fit(const SeriesType &series, int degree)
            {
                assert(series.size() > 0);
                assert(degree >= 0);

                // Set up least squares matrix
                auto m0 = matrix::Matrix(degree+1,degree+2);

                // Incorporate xs
                for(int i=0; i<=degree; i++)
                {
                    for(int j=0; j<=degree; j++)
                    {
                        for(auto pair : series)
                        {
                            auto x = pair.first;
                            m0.set(i, j, m0.get(i,j) + pow(x, i + j));
                        }
                    }
                }

                // Incorporate ys
                for(int i=0; i<=degree; i++)
                {
                    for(int j=0; j<series.size(); j++)
                    {
                        auto x = series[j].first;
                        auto y = series[j].second;
                        m0.set(i, degree+1, m0.get(i,degree+1) + pow(x,i) * y);
                    }
                }

                // Perform gaussian elimination to solve the system
                matrix::GaussianElimination::gaussianElimination(m0);

                // The last column should yield the coefficients of the polynomial
                std::vector<double> coeffs;
                for(int i=0; i<=degree; i++)
                {
                    coeffs.push_back(m0.get(i, degree+1));
                }

                return coeffs;
            }

            /*! Fits a collection of points to a polynomial of given degree.
             	 This method uses a least-squares technique. It is not advised,
             	 both from the perspective of computational complexity,
             	 numeric stability, as well as for the sake of accuracy to use
             	 high-degree polynomials.
             */
            static SeriesType smooth(const SeriesType &series, int degree)
            {
                // asserts
                assert(series.size() > 0);
                assert(degree >= 0);

                // fit polynomial
                auto poly = PolynomialSmoothing::fit(series, degree);

                // utility function to evaluate polynomial
                auto eval = [&poly](double x)
                {
                    auto y = 0.0;
                    for(int i=0; i<poly.size(); i++)
                    {
                        y += poly[i] * pow(x, i);
                    }
                    return y;
                };

                // create approximation
                SeriesType approx;
                for(int i=0; i<series.size(); i++)
                {
                    auto x = series[i].first;
                    approx.push_back(std::make_pair(x, eval(x)));
                }

                // return
                return approx;
            }

    };
}

#endif // POLYFIT_HPP
