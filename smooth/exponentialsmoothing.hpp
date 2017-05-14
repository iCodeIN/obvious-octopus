#pragma once
#ifndef SMOOTH_EXPONENTIALSMOOTHING_HPP
#define SMOOTH_EXPONENTIALSMOOTHING_HPP

#include "ismooth.hpp"

#include <assert.h>

namespace smooth
{
    /*! Exponential smoothing is a rule of thumb technique for smoothing time series data,
        particularly for recursively applying low-pass filters with exponential window
       functions. Such techniques have broad application that is not intended to be strictly
     accurate or reliable for every situation. It is an easily learned and easily applied procedure
     for approximately calculating or recalling some value, or for making some determination
     based on prior assumptions by the user, such as seasonality.
     */
    class ExponentialSmoothing final : public ISmooth
    {
        public:

            /*! Construct an exponential smoothing algorithm
             */
            explicit ExponentialSmoothing(double alpha)
                : alpha(alpha)
            {
                assert(alpha >= 0 && alpha <= 1);
            }

            /*! Default destructor
             */
            virtual ~ExponentialSmoothing() = default;

            /*! Prohibit const copy constructor
             */
            ExponentialSmoothing(const ExponentialSmoothing&) = delete;

            /*! Prohibit copy constructor
             */
            ExponentialSmoothing(ExponentialSmoothing&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const ExponentialSmoothing&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(ExponentialSmoothing&) = delete;

            /*! Define a Series
             */
            using SeriesType = std::vector<std::pair<double,double>>;


            /*! Apply exponential smoothing.
             */
            SeriesType smooth(const SeriesType &xs) const
            {
                // asserts
                assert(xs.size() > 0);

                std::vector<std::pair<double,double>> retval;
                retval.push_back(xs[0]);
                for(int i=1; i<xs.size(); i++)
                {
                    auto y = xs[i].second * alpha + retval[i-1].second * (1.0 - alpha);
                    retval.push_back(std::make_pair(xs[i].first, y));
                }
                return retval;
            }

        private:
            double alpha;

    };
}

#endif // SMOOTH_EXPONENTIALSMOOTHING_HPP
