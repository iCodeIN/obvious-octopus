#pragma once
#ifndef META_SIMULATEDANNEALING_HPP
#define META_SIMULATEDANNEALING_HPP

#include "abstractmetaheuristic.hpp"

namespace meta
{
    /*! Simulated annealing (SA) is a probabilistic technique for approximating the global optimum of a given function.
        Specifically, it is a metaheuristic to approximate global optimization in a large search space.
        It is often used when the search space is discrete (e.g., all tours that visit a given set of cities).
        For problems where finding the precise global optimum is less important than finding an acceptable local optimum in a fixed amount of time, simulated annealing may be preferable to alternatives such as gradient descent.
        Simulated annealing interprets slow cooling as a slow decrease in the probability of accepting worse solutions as it explores the solution space.
        Accepting worse solutions is a fundamental property of metaheuristics because it allows for a more extensive search for the optimal solution.
        The method was independently described by Scott Kirkpatrick, C. Daniel Gelatt and Mario P. Vecchi in 1983, and by Vlado Černý in 1985.
        The method is an adaptation of the Metropolis–Hastings algorithm, a Monte Carlo method to generate sample states of a thermodynamic system, invented by M.N. Rosenbluth and published by N. Metropolis et al. in 1953.
     */
    class SimulatedAnnealing final : public AbstractMetaHeuristic
    {
        public:
            // --- AbstractMetaHeuristic ---
            virtual std::vector<double> findMax(const TupleType &seed, const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps) override
            {
                // asserts
                assert(maxGenerations > 0);
                assert(seed.size() == eps.size());
                assert(seed.size() > 0);
                assert(std::find_if(eps.begin(), eps.end(), [](const double d)
                {
                    return d < 0;
                }) == eps.end());
                assert(std::find_if(domains.begin(), domains.end(), [](const DomainType &d)
                {
                    return std::get<0>(d) >= std::get<1>(d);
                }) == domains.end());

                // shortcut
                int N = eps.size();

                // random start
                auto retval(seed);
                m_maxF = function(seed);

                // walk around the neighbourhood
                for(int i=0; i<maxGenerations; i++)
                {
                    // generate neighbour
                    TupleType nb = neighbour(retval, domains, eps);

                    // update best
                    if(function(nb) > function(retval))
                    {
                        retval = nb;
                    }
                    else
                    {
                        auto q = quality(nb, function);
                        auto t = temperature(i, maxGenerations - 1);
                        if(sometimes(q * t))
                        {
                            retval = nb;
                        }
                    }
                }

                // return best result so far
                return retval;
            }

        private:

            // --- methods ---

            /*!
             */
            double quality(const TupleType &t, const FunctionType &function) const
            {
                auto f = function(t);
                if(f > m_maxF)
                {
                    m_maxF = f;
                }
                auto q      = 1.0 - std::abs(f - m_maxF);
                auto alpha  = 0.95;
                return q * alpha + (1.0 - alpha);
            }

            /*!
             */
            double temperature(int generation, int maxGeneration) const
            {
                auto PI = 3.141592;
                auto rGen = (double) generation / maxGeneration;
                return (cos(rGen*PI) + 1.0) * 0.5;
            }

            // --- members ---
            mutable double m_maxF;
    };
}

#endif // META_SIMULATEDANNEALING_HPP
