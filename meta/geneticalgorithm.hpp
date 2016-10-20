#pragma once
#ifndef GENETICALGORITHM_HPP
#define GENETICALGORITHM_HPP

#include <iostream> // debug

#include "abstractmetaheuristic.hpp"

namespace meta
{
    /*! In the field of mathematical optimization, a genetic algorithm (GA) is a search heuristic that mimics the process of natural selection.
        This heuristic (also sometimes called a metaheuristic) is routinely used to generate useful solutions to optimization and search problems.
        Genetic algorithms belong to the larger class of evolutionary algorithms (EA), which generate solutions to optimization problems using techniques inspired by natural evolution,
        such as inheritance, mutation, selection and crossover.
     */
    class GeneticAlgorithm final : public AbstractMetaHeuristic
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

                /* calculate initial search space size
                   for performance reasons, we decide to explore only 2% of the total solution space at a time
                 */
                auto populationSize = 1;
                for(int i=0; i<domains.size(); i++)
                {
                    auto d = domains[i];
                    auto min = std::get<0>(d);
                    auto max = std::get<1>(d);
                    populationSize *= ((std::abs(min) + max) / eps[i]) * 0.02;
                }
                populationSize = std::min(std::max(populationSize, 2), 128);

                /* generate initial search space
                 */
                std::vector<TupleType> solutions;
                solutions.reserve(populationSize);
                for(int i=0; i<populationSize; i++)
                {
                    solutions.push_back(neighbour(seed, domains, eps));
                }

                // perform iterations
                auto retval = best(solutions, function);
                for(int i=0; i<maxGenerations; i++)
                {
                    // calculate next generation
                    nextSolutions(solutions, function, domains, eps);

                    // keep track of best individual
                    auto generationBest = best(solutions, function);
                    std::cout << "gen : " << i << ", max : " << function(generationBest) << std::endl;
                    if(function(generationBest) > function(retval))
                    {
                        retval = generationBest;
                    }

                }

                // return
                return retval;
            }

        private:
            // --- methods ---

            /*! \return the solution with the highest value for the objective function
             */
            TupleType best(const std::vector<TupleType> &solutions, const FunctionType &function) const
            {
                assert(solutions.size() > 0);

                auto maxF = function(solutions[0]);
                auto maxFSolution = solutions[0];

                std::for_each(solutions.begin(), solutions.end(), [&maxF, &maxFSolution, &function](const TupleType &t)
                {
                    auto f = function(t);
                    if(f > maxF)
                    {
                        maxF = f;
                        maxFSolution = t;
                    }
                });

                return maxFSolution;
            }

            /*! Randomly select a solution from the solution space, with likelyhood proportional to the objective function
             */
            TupleType randSolution(const std::vector<TupleType> &solutions, const FunctionType &function)
            {
                assert(solutions.size() > 0);

                // determine upper and lower boundary of function for the given solutions
                auto minF = function(solutions[0]);
                auto maxF = (double) minF;
                std::for_each(solutions.begin(), solutions.end(), [&minF, &maxF, &function](const TupleType &t)
                {
                    auto f = function(t);
                    minF = std::min(minF, f);
                    maxF = std::max(maxF, f);
                });

                // if upper and lower bound are the same, all solutions have the same fitness, and we can simply select one at random
                if(minF != maxF)
                {
                    // calculate normalization constant
                    auto totalF = 0.0;
                    std::for_each(solutions.begin(), solutions.end(), [&totalF, &function, minF, maxF](const TupleType &t)
                    {
                        auto p = (function(t) + std::abs(minF)) / (maxF + std::abs(minF));
                        totalF+= p;
                    });

                    // select a solution
                    for(int i=0; i<solutions.size(); i++)
                    {
                        auto p = (function(solutions[i]) + std::abs(minF)) / (maxF + std::abs(minF));
                        p /= totalF;
                        assert(p >= 0);
                        if(sometimes(p))
                        {
                            return solutions[i];
                        }
                    }

                }

                // select random solution
                std::uniform_int_distribution<int> dist(0, solutions.size()-1);
                return solutions[dist(m_generator)];
            }

            /*!
             */
            std::vector<TupleType> combine(TupleType &t0, TupleType &t1) const
            {
                assert(t0.size() > 0);
                assert(t0.size() == t1.size());

                TupleType child0;
                TupleType child1;
                std::uniform_int_distribution<int> dist(0, t0.size()-1);
                auto index = dist(m_generator);

                for(int i=0; i<t0.size(); i++)
                {
                    auto swap = (i < index) || (t0.size() == 1 && sometimes(0.5));
                    if(!swap)
                    {
                        child0.push_back(t0[i]);
                        child1.push_back(t1[i]);
                    }
                    else
                    {
                        child0.push_back(t1[i]);
                        child1.push_back(t0[i]);
                    }
                }
                std::vector<TupleType> retVal;
                retVal.push_back(child0);
                retVal.push_back(child1);
                return retVal;
            }

            /*!
             */
            void nextSolutions(std::vector<TupleType> &solutions, const FunctionType &function, const std::vector<DomainType> &domains, const std::vector<double> &eps)
            {
                // asserts
                assert(solutions.size() > 0);
                assert(std::find_if(eps.begin(), eps.end(), [](const double d)
                {
                    return d < 0;
                }) == eps.end());

                // datastructure to hold new solutions
                std::vector<TupleType> temp;

                // crossover and mutation
                while(temp.size() < solutions.size())
                {
                    auto parent0 = randSolution(solutions, function);
                    auto parent1 = randSolution(solutions, function);
                    for(auto &child : combine(parent0, parent1))
                    {
                        temp.push_back(neighbour(child, domains, eps));
                    }
                }

                // add to current population
                std::for_each(solutions.begin(), solutions.end(), [&temp](const TupleType &t)
                {
                    temp.push_back(t);
                });

                // select best
                auto N = solutions.size();
                solutions.clear();
                for(int i=0; i<N; i++)
                {
                    solutions.push_back(randSolution(temp, function));
                }
            }

            // --- members ---
            mutable std::default_random_engine    m_generator;
    };
}

#endif //GENETICALGORITHM_HPP
