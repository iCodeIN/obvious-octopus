#pragma once
#ifndef META_LOCALSEARCH_H
#define META_LOCALSEARCH_H

#include "abstractmetaheuristic.hpp"

namespace meta
{
    /*!  Local search is a heuristic method for solving computationally hard optimization problems.
         Local search can be used on problems that can be formulated as finding a solution maximizing a criterion among a number of candidate solutions.
         Local search algorithms move from solution to solution in the space of candidate solutions (the search space) by applying local changes, until a solution deemed optimal is found or a time bound is elapsed.
         Local search algorithms are widely applied to numerous hard computational problems, including problems from computer science (particularly artificial intelligence),
         mathematics, operations research, engineering, and bioinformatics. Examples of local search algorithms are WalkSAT and the 2-opt algorithm for the Traveling Salesman Problem.
     */
    class LocalSearch final : public AbstractMetaHeuristic
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
                }

                // return best result so far
                return retval;
            }
    };
}

#endif // META_LOCALSEARCH_H
