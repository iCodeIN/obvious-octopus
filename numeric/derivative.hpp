#pragma once
#ifndef NUMERIC_DERIVATIVE_HPP
#define NUMERIC_DERIVATIVE_HPP

#include <functional>
#include <math.h>

namespace numeric
{
    /*! In numerical analysis, numerical differentiation describes algorithms for estimating the derivative of a mathematical function
        or function subroutine using values of the function and perhaps other knowledge about the function.
     */
    class Derivative
    {
        public:
            /*! Prohibit construction of Derivative. This class offers only static methods.
             */
            explicit Derivative() = delete;

            /*! Define a function over the real numbers
             */
            using RealFunctionType = std::function<double(double)>;

            /*! \return a numeric approximation of the derivative
             */
            static RealFunctionType derivative(const RealFunctionType &f)
            {
                auto delta = pow(10,-10);
                return [f, delta](double x)
                {
                    return (f(x - delta) + f(x + delta)) / (2 * delta);
                };
            }
    };
}

#endif // NUMERIC_DERIVATIVE_HPP
