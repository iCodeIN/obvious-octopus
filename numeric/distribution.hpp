#pragma once
#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include <algorithm>
#include <assert.h>
#include <map>
#include <vector>

namespace numeric
{
    /*! In probability and statistics, a probability distribution is a mathematical description of a random phenomenon in terms of the probabilities of events.
        Examples of random phenomena include the results of an experiment or survey.
        A probability distribution is defined in terms of an underlying sample space, which is the set of all possible outcomes of the random phenomenon being observed.
        The sample space may be the set of real numbers or a higher-dimensional vector space, or it may be a list of non-numerical values.
     */
    template <typename T>
    class Distribution final
    {
        public:
            /*!  The count of how many data values fall into a certain class constitutes the frequency for this class
             */
            using AbsoluteDistributionType = std::map<T, int>;

            /*! Relative frequency is a measure of what proportion or percent of the data values fall into a particular class
             */
            using RelativeDistributionType = std::map<T, double>;

            /*!
             */
            static const RelativeDistributionType makeRelative(const AbsoluteDistributionType &d)
            {
                RelativeDistributionType retval;
                double totalMass = 0.0;
                for(auto pair : d)
                {
                    assert(pair.second >= 0);
                    totalMass += pair.second;
                }
                for(auto pair : d)
                {
                    retval[pair.first] = totalMass == 0 ? 0.0 : (pair.second / totalMass);
                }
                return retval;
            }

            /*!
             */
            static const AbsoluteDistributionType makeAbsolute(const RelativeDistributionType &d, int total)
            {
                assert(total >= 0);
                AbsoluteDistributionType retval;
                for(auto pair : d)
                {
                    assert(pair.second >= 0);
                    assert(pair.second >= 0);
                    retval[pair.first] = (pair.second * total);
                }
                return retval;
            }

            /*! In colloquial language, an average is the sum of a list of numbers divided by the number of numbers in the list.
                In mathematics and statistics, this would be called the arithmetic mean.
             */
            static const double avg(const RelativeDistributionType &d)
            {
                auto total = 0.0;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    total += pair.second;
                }
                return d.size() == 0 ? 0.0 : (total / d.size());
            }

            /*! In colloquial language, an average is the sum of a list of numbers divided by the number of numbers in the list.
                In mathematics and statistics, this would be called the arithmetic mean.
             */
            static const double avg(const AbsoluteDistributionType &d)
            {
                auto total = 0.0;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    total += pair.second;
                }
                return d.size() == 0 ? 0.0 : (total / d.size());
            }

            /*! In mathematics and statistics, deviation is a measure of difference between the observed value of a variable and some other value, often that variable's mean.
                The sign of the deviation (positive or negative), reports the direction of that difference (the deviation is positive when the observed value exceeds the reference value).
                The magnitude of the value indicates the size of the difference.
             */
            static const double dev(const RelativeDistributionType &d)
            {
                auto average = avg(d);
                auto retval = 0.0;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    retval += (pair.second - average);
                }
                return retval / d.size();
            }

            /*! In mathematics and statistics, deviation is a measure of difference between the observed value of a variable and some other value, often that variable's mean.
                The sign of the deviation (positive or negative), reports the direction of that difference (the deviation is positive when the observed value exceeds the reference value).
                The magnitude of the value indicates the size of the difference.
             */
            static const double dev(const AbsoluteDistributionType &d)
            {
                auto average = avg(d);
                auto retval = 0.0;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    retval += (pair.second - average);
                }
                return retval / d.size();
            }

            /*! 
             */
            static const RelativeDistributionType left(const RelativeDistributionType &d, double p)
            {
                assert(p >= 0 && p <= 1.0);

                // convert to vector
                std::vector<std::pair<T, double>> temp;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    temp.push_back(pair);
                }

                // sort
                std::sort(temp.begin(), temp.end(), [](const std::pair<T,double> &p0, const std::pair<T,double> &p1)
                {
                    return p0.second < p1.second;
                });

                // convert back to map
                RelativeDistributionType retval;
                auto totalMass = 0.0;
                auto index = 0;
                while(totalMass < p && index < temp.size())
                {
                    auto &pair = temp[index];
                    retval[pair.first] = pair.second;
                    totalMass += pair.second;
                    index++;
                }

                // return
                return retval;
            }

            /*!
             */
            static const RelativeDistributionType right(const RelativeDistributionType &d, double p)
            {
                assert(p >= 0 && p <= 1.0);

                // convert to vector
                std::vector<std::pair<T, double>> temp;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    temp.push_back(pair);
                }

                // sort
                std::sort(temp.begin(), temp.end(), [](const std::pair<T,double> &p0, const std::pair<T,double> &p1)
                {
                    return p0.second < p1.second;
                });

                // convert back to map
                RelativeDistributionType retval;
                auto totalMass = 0.0;
                auto index = temp.size() - 1;
                while(totalMass < p && index >= 0)
                {
                    auto &pair = temp[index];
                    retval[pair.first] = pair.second;
                    totalMass += pair.second;
                    index--;
                }

                // return
                return retval;
            }

            /*! 
             */
            static const AbsoluteDistributionType left(const AbsoluteDistributionType &d, double p)
            {
                auto total = 0;
                return makeAbsolute(left(makeRelative(d), p), total);
            }

            /*!
             */
            static const AbsoluteDistributionType right(const AbsoluteDistributionType &d, double p)
            {
                auto total = 0;
                return makeAbsolute(right(makeRelative(d), p), total);
            }

    };
}

#endif // DISTRIBUTION_HPP