#pragma once
#ifndef ABSTRACTMETAHEURISTIC_HPP
#define ABSTRACTMETAHEURISTIC_HPP

#include <algorithm>
#include <assert.h>
#include <functional>
#include <random>
#include <tuple>
#include <vector>

namespace meta
{
    /*! In computer science and mathematical optimization, a metaheuristic is a higher-level procedure or heuristic designed to find, generate, or select a heuristic (partial search algorithm)
        that may provide a sufficiently good solution to an optimization problem, especially with incomplete or imperfect information or limited computation capacity.
        Metaheuristics sample a set of solutions which is too large to be completely sampled. Metaheuristics may make few assumptions about the optimization problem being solved,
        and so they may be usable for a variety of problems.
     */
    class AbstractMetaHeuristic
    {
        public:

            /*! Define the input/output type for the optimization problem
             */
            using TupleType = std::vector<double>;

            /*! Define the objective function, which maps input to a double (representing the suitability of the given input)
             */
            using FunctionType = std::function<double(const TupleType &input)>;

            /*! Define a specialization of the objective function, for univariate functions
             */
            using UnivariateFunctionType = std::function<double(double input)>;

            /*! Define the Domain type, which is used for specifying the domain of each variable being modified
             */
            using DomainType = std::tuple<double,double>;

            /*! Find a maximum of the given function
                \param[in] seed              the initial search location
                \param[in] function          the function to be maximized
                \param[in] maxGenerations    the number of generations the specific metaheuristic is allowed to work on the problem
                \param[in] domains           the domain of each variable being modified
                \param[in] eps               the perturbations (absolute value) that the metaheuristic is allowed to make per input value, per generation
             */
            virtual TupleType findMax(const TupleType &seed, const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps) = 0;

            /*! Find a maximum of the given function
                \param[in] function          the function to be maximized
                \param[in] domains           the domain of each variable being modified
                \param[in] maxGenerations    the number of generations the specific metaheuristic is allowed to work on the problem
                \param[in] eps               the perturbations (absolute value) that the metaheuristic is allowed to make per input value, per generation
             */
            virtual TupleType findMaxUnseeded(const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps)
            {
                // init random
                srand(time(0));

                // shortcut
                int N = eps.size();

                // generate seed
                auto seed = randTuple(domains);

                // redirect
                return findMax(seed, function, domains, maxGenerations, eps);
            }

            /*! Find a minimum of the given function
                \param[in] seed              the initial search location
                \param[in] function          the function to be maximized
                \param[in] domains           the domain of each variable being modified
                \param[in] maxGenerations    the number of generations the specific metaheuristic is allowed to work on the problem
                \param[in] eps               the perturbations (absolute value) that the metaheuristic is allowed to make per input value, per generation
             */
            TupleType findMin(const TupleType &seed, const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps)
            {
                auto f = [&function](const TupleType &input)
                {
                    return -function(input);
                };
                return findMax(seed, f, domains, maxGenerations, eps);
            }

            /*! Find a minimum of the given function
                \param[in] function          the function to be maximized
                \param[in] domains           the domain of each variable being modified
                \param[in] maxGenerations    the number of generations the specific metaheuristic is allowed to work on the problem
                \param[in] eps               the perturbations (absolute value) that the metaheuristic is allowed to make per input value, per generation
             */
            TupleType findMinUnseeded(const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps)
            {
                auto f = [&function](const TupleType &input)
                {
                    return -function(input);
                };
                return findMaxUnseeded(f, domains, maxGenerations, eps);
            }

            /*! Find a maximum of the given univariate function
                \param[in] function          the univariate function to be maximized
                \param[in] domain            the domain of the variable being modified
                \param[in] maxGenerations    the number of generations the specific metaheuristic is allowed to work on the problem
                \param[in] eps               the perturbations (absolute value) that the metaheuristic is allowed to make per generation
             */
            double findUnivariateMaxUnseeded(const UnivariateFunctionType &function, const DomainType &domain, int maxGenerations, double eps)
            {
                auto f = [&function](const TupleType &input)
                {
                    return function(input[0]);
                };

                std::vector<double> epsVector;
                epsVector.push_back(eps);

                std::vector<DomainType> domains;
                domains.push_back(domain);

                return findMaxUnseeded(f, domains, maxGenerations, epsVector)[0];
            }

            /*! Find a minimum of the given univariate function
                \param[in] function          the univariate function to be maximized
                \param[in] domain            the domain of the variable being modified
                \param[in] maxGenerations    the number of generations the specific metaheuristic is allowed to work on the problem
                \param[in] eps               the perturbations (absolute value) that the metaheuristic is allowed to make per generation
             */
            double findUnivariateMinUnseeded(const UnivariateFunctionType &function, const DomainType &domain, int maxGenerations, double eps)
            {
                auto f = [&function](const TupleType &input)
                {
                    return -function(input[0]);
                };

                std::vector<double> epsVector;
                epsVector.push_back(eps);

                std::vector<DomainType> domains;
                domains.push_back(domain);

                return findMaxUnseeded(f, domains, maxGenerations, epsVector)[0];
            }

            /*
             * UTILITY METHODS
             */

            /*! \return true with a probability p, false otherwise
             */
            bool sometimes(double p) const
            {
                assert(p >= 0.0 && p <= 1.0);
                auto dist = std::uniform_real_distribution<double>(0, 1);
                auto p2 = dist(m_generator);
                return p > (1.0 - p2);
            }

            /*! \return a randomly generated TupleType with length n
             */
            TupleType randTuple(const std::vector<DomainType> &domains) const
            {
                // asserts
                assert(domains.size() >=0 );
                assert(std::find_if(domains.begin(), domains.end(), [](const DomainType &d)
                {
                    return std::get<0>(d) >= std::get<1>(d);
                }) == domains.end());

                // shortcuts
                auto N = domains.size();

                // create tuple
                TupleType t;
                for(int i=0; i<N; i++)
                {
                    // setup uniform distribution within domain
                    auto low = std::get<0>(domains[i]);
                    auto upr = std::get<1>(domains[i]);
                    std::uniform_real_distribution<double> distribution(low, upr);

                    // generate random number from distribution
                    auto d = distribution(m_generator);

                    // assign
                    t.push_back(d);
                }

                return t;
            }

            /*! \return a slightly perturbated version of a TupleType
             */
            TupleType neighbour(const TupleType &t, const std::vector<DomainType> &domains, const std::vector<double> &eps)
            {
                // asserts
                assert(t.size() == eps.size());
                assert(t.size() == domains.size());
                assert(t.size() >= 0);
                assert(std::find_if(eps.begin(), eps.end(), [](const double d)
                {
                    return d < 0;
                }) == eps.end());
                assert(std::find_if(domains.begin(), domains.end(), [](const DomainType &d)
                {
                    return std::get<0>(d) >= std::get<1>(d);
                }) == domains.end());

                // shortcut
                auto N = t.size();

                // copy tuple
                TupleType retval(t);

                // modify some indices
                std::uniform_int_distribution<int> indexDistribution(0, N-1);

                for(int i=0; i<1+indexDistribution(m_generator); i++)
                {
                    auto index = indexDistribution(m_generator);
                    auto low = std::get<0>(domains[index]);
                    auto upr = std::get<1>(domains[index]);
                    retval[index] += sometimes(0.5) ? eps[index] : -eps[index];
                    retval[index] = std::max(std::min(retval[index], upr), low);
                }

                // return
                return retval;
            }

            // --- members ---
            mutable std::default_random_engine    m_generator; //!< random number generator used to generate seeds for the metaheuristic
    };
}

#endif // ABSTRACTMETAHEURISTIC_HPP

