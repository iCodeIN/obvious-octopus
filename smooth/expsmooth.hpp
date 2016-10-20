#pragma once
#ifndef EXPSMOOTH_HPP
#define EXPSMOOTH_HPP

#include <assert.h>
#include <vector>

namespace smooth
{
    /*! Exponential smoothing is a rule of thumb technique for smoothing time series data,
        particularly for recursively applying low-pass filters with exponential window
       functions. Such techniques have broad application that is not intended to be strictly
     accurate or reliable for every situation. It is an easily learned and easily applied procedure
     for approximately calculating or recalling some value, or for making some determination
     based on prior assumptions by the user, such as seasonality.
     */
    class ExponentialSmoothing final
    {
        public:
            /*! Prohibit construction of ExponentialSmoothing. This class offers only static methods.
             */
            explicit ExponentialSmoothing() = delete;

            /*! Apply exponential smoothing.
            	 */
            static std::vector<double> smooth(const std::vector<double> &xs, double alpha)
            {
                // asserts
                assert(alpha >= 0 && alpha <= 1);
                assert(xs.size() > 0);

                std::vector<double> ys;
                ys.push_back(xs[0]);
                for(int i=1; i<xs.size(); i++)
                {
                    auto v = xs[i] * alpha + ys[i-1] * (1.0 - alpha);
                    ys.push_back(v);
                }
                return ys;
            }

    };
}

#endif // EXPSMOOTH_HPP
