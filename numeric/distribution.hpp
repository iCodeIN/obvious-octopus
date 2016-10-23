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

            /*!
             */
            using DistributionType = std::map<X, Y>;

            /*!
             */
            static const DistributionType relative(const DistributionType &d)
            {
                DistributionType retval;
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

            /*!
             */
            static const DistributionType absolute(const DistributionType &d, const Y &total)
            {
                assert(total >= 0);
                DistributionType retval;
                for(auto pair : d)
                {
                    assert(pair.second >= 0);
                    retval[pair.first] = (pair.second * total);
                }
                return retval;
            }

            /*!
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

            /*!
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