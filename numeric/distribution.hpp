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
    template <typename X, typename Y>
    class Distribution
    {
        public:

            /*! define a distribution type
             */
            using DistributionType = std::map<X, Y>;

            /*! \return the relative frequencies that correspond to the absolute frequencies in the given distribution
             */
            static const std::map<X,double> relative(const DistributionType &d)
            {
                std::map<X, double> retval;
                Y totalMass = 0;
                for(auto pair : d)
                {
                    assert(pair.second >= 0);
                    totalMass += pair.second;
                }
                for(auto pair : d)
                {
                    retval[pair.first] = totalMass == 0 ? 0 : (pair.second / totalMass);
                }
                return retval;
            }

            /*! \return the absolute frequences that correspond to the relative frequencies in the given relative distribution
             */
            static const std::map<X,int> absolute(const DistributionType &d, const Y &total)
            {
                assert(total >= 0);
                std::map<X, int> retval;
                for(auto pair : d)
                {
                    assert(pair.second >= 0);
                    retval[pair.first] = (pair.second * total);
                }
                return retval;
            }

            /*! \return the average of a distribution.
                In colloquial language, an average is the sum of a set of numbers divided by the number of numbers in the set. In mathematics and statistics, this would be called the arithmetic mean.
             */
            static const Y avg(const DistributionType &d)
            {
                Y total = 0;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    total += pair.second;
                }
                return d.size() == 0 ? ((Y) 0) : (total / d.size());
            }

            /*! \return the deviance of a distribution
                In mathematics and statistics, deviation is a measure of difference between the observed value of a variable and some other value, often that variable's mean.
                The sign of the deviation (positive or negative), reports the direction of that difference (the deviation is positive when the observed value exceeds the reference value).
                The magnitude of the value indicates the size of the difference.
             */
            static const Y dev(const DistributionType &d)
            {
                auto average = avg(d);
                Y retval = 0;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    retval += (pair.second - average);
                }
                return (Y) (retval / d.size());
            }

            /*!
             */
            static const DistributionType left(const DistributionType &d, double p)
            {
                assert(p >= 0 && p <= 1.0);

                // convert to vector
                std::vector<std::pair<X, Y>> temp;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    temp.push_back(pair);
                }

                // sort
                std::sort(temp.begin(), temp.end(), [](const std::pair<X, Y> &p0, const std::pair<X, Y> &p1)
                {
                    return p0.second < p1.second;
                });

                // convert back to map
                DistributionType retval;
                Y totalMass = 0.0;
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
            static const DistributionType right(const DistributionType &d, double p)
            {
                assert(p >= 0 && p <= 1.0);

                // convert to vector
                std::vector<std::pair<X, Y>> temp;
                for(auto &pair : d)
                {
                    assert(pair.second >= 0);
                    temp.push_back(pair);
                }

                // sort
                std::sort(temp.begin(), temp.end(), [](const std::pair<X, Y> &p0, const std::pair<X, Y> &p1)
                {
                    return p0.second < p1.second;
                });

                // convert back to map
                DistributionType retval;
                Y totalMass = 0.0;
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


    };
}

#endif // DISTRIBUTION_HPP