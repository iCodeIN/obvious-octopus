#pragma once
#ifndef META_TABUSEARCH_HPP
#define META_TABUSEARCH_HPP

#include "abstractmetaheuristic.hpp"

namespace meta
{
    /*! Tabu search, created by Fred W. Glover in 1986[1] and formalized in 1989,[2][3] is a metaheuristic search method employing local search methods used for mathematical optimization.
        Local (neighborhood) searches take a potential solution to a problem and check its immediate neighbors (that is, solutions that are similar except for one or two minor details)
        in the hope of finding an improved solution. Local search methods have a tendency to become stuck in suboptimal regions or on plateaus where many solutions are equally fit.
        Tabu search enhances the performance of local search by relaxing its basic rule.
        First, at each step worsening moves can be accepted if no improving move is available (like when the search is stuck at a strict local minimum).
        In addition, prohibitions (henceforth the term tabu) are introduced to discourage the search from coming back to previously-visited solutions.
     */
    class TabuSearch final : public AbstractMetaHeuristic
    {
        public:

            // --- AbstractMetaHeuristic ---
            virtual TupleType findMax(const TupleType &seed, const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps) override
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
                auto N = eps.size();

                // setup tabu
                std::vector<TupleType> tabu;
                auto tabuTenure = 32;
                tabu.reserve(tabuTenure);

                // random start
                auto retval(seed);

                // iterations
                std::vector<TupleType> nbs;
                for(int i=0; i<maxGenerations; i++)
                {
                    // generate neighbourhood
                    nbs.clear();
                    for(int j=0; j<64; j++)
                    {
                        nbs.push_back(neighbour(retval, domains, eps));
                    }

                    // pick best neighbour that is not in tabu list
                    auto maxF = function(nbs[0]);
                    auto maxFSolution = nbs[0];
                    std::for_each(nbs.begin(), nbs.end(), [&maxF, &maxFSolution, &function, &tabu](const TupleType &t)
                    {
                        auto f = function(t);
                        if(std::find(tabu.begin(), tabu.end(), t) != tabu.end() && f > maxF)
                        {
                            maxF = f;
                            maxFSolution = t;
                        }
                    });

                    // add neighbour to tabu list
                    retval = maxFSolution;
                    tabu.push_back(maxFSolution);

                    // trim tabu list if needed
                    if(tabu.size() >= tabuTenure)
                    {
                        tabu.erase(tabu.begin());
                    }

                }

                // return
                return retval;
            }
    };
}

#endif // META_TABUSEARCH_HPP
