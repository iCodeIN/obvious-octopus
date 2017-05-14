#pragma once
#ifndef SMOOTH_ISMOOTH_HPP
#define SMOOTH_ISMOOTH_HPP

#include <vector>

namespace smooth
{
    /*! In statistics and image processing, to smooth a data set is to create an approximating function that attempts to capture important patterns in the data,
        while leaving out noise or other fine-scale structures/rapid phenomena.
        In smoothing, the data points of a signal are modified so individual points (presumably because of noise) are reduced,
        and points that are lower than the adjacent points are increased leading to a smoother signal.
        Smoothing may be used in two important ways that can aid in data analysis by being able to extract more information from the data as long as
        the assumption of smoothing is reasonable and by being able to provide analyses that are both flexible and robust.
     */
    class ISmooth
    {
        public:

            /*! Default constructor
             */
            explicit ISmooth() = default;

            /*! Default destructor
             */
            virtual ~ISmooth() = default;

            /*! Prohibit const copy constructor
             */
            ISmooth(const ISmooth&) = delete;

            /*! Prohibit copy constructor
             */
            ISmooth(ISmooth&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const ISmooth&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(ISmooth&) = delete;

            /*! Define a Series
             */
            using SeriesType = std::vector<std::pair<double,double>>;

            /*! Apply smoothing
             */
            virtual SeriesType smooth(const SeriesType &series) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // SMOOTH_POLYSMOOTH_HPP
