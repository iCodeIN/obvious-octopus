#pragma once
#ifndef PERMUTATIONSEARCH_HPP
#define PERMUTATIONSEARCH_HPP

#include "meta/abstractmetaheuristic.hpp"

#include <memory>

namespace meta
{
    /*! This class acts as a wrapper around AbstractMetaHeuristic. It provides specific functionality for solving permutation problems.
        A problem is considered a permutation problem iff a permutation of the numbers 0 .. n is to be found so that a certain criterium is met (often expressed as a function to be maximized or minimized.)
        This class will use the wrapped AbstractMetaHeuristic (and some utility methods for conversion) to solve the problem.
     */
    class PermutationSearch final : public AbstractMetaHeuristic
    {
        public:

            /*! constructor
                \param[in] heuristic the wrapped heuristic
             */
            explicit PermutationSearch(std::unique_ptr<AbstractMetaHeuristic> heuristic)
                : m_heuristic(std::move(heuristic))
            {
            }

            // --- AbstractMetaHeuristic ---
            virtual TupleType findMax(const TupleType &seed, const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps)
            {
                auto modF = [this, &function](const TupleType &t)
                {
                    return function(toPermutationVector(t));
                };
                return toPermutationVector(m_heuristic.get()->findMax(seed, modF, domains, maxGenerations, eps));
            }

            /*! Define the input type for this kind of metaheuristic
             */
            using IntTupleType = std::vector<int>;

            /*! Define the type of the function to be optimized for this kind of metaheuristic
             */
            using IntFunctionType = std::function<double(IntTupleType)>;

            /*! Find a permutation of 0 .. n-1 (where n is the vectorLength) so that the given function is maximized
                \param[in] function         the function to be maximized
                \param[in] vectorLength     the length of the permutation vector to be generated (also the number of arguments of the function)
                \param[in] maxGenerations   the number of generations the specific metaheuristic is allowed to work on the problem
             */
            IntTupleType findIntMax(const IntFunctionType &function, int vectorLength, int maxGenerations)
            {
                // function transform
                auto modF = [this, &function](const TupleType &t)
                {
                    return function(toIntTuple(t));
                };

                // setup domains
                std::vector<DomainType> domains;
                for(int i=0; i<vectorLength; i++)
                {
                    domains.push_back(std::make_tuple(0, vectorLength - 1));
                }

                // setup eps
                std::vector<double> eps;
                for(int i=0; i<vectorLength; i++)
                {
                    eps.push_back(1);
                }

                // setup seed
                std::vector<double> seed;
                for(int i=0; i<vectorLength; i++)
                {
                    seed.push_back(i);
                }

                // redirect call
                return toIntTuple(findMax(seed, modF, domains, maxGenerations, eps));
            }

            /*! Find a permutation of 0 .. n-1 (where n is the vectorLength) so that the given function is minimized
                \param[in] function         the function to be minimized
                \param[in] vectorLength     the length of the permutation vector to be generated (also the number of arguments of the function)
                \param[in] maxGenerations   the number of generations the specific metaheuristic is allowed to work on the problem
             */
            IntTupleType findIntMin(const IntFunctionType &function, int vectorLength, int maxGenerations)
            {
                // function transform
                auto modF = [this, &function](const TupleType &t)
                {
                    return function(toIntTuple(t));
                };

                // setup domains
                std::vector<DomainType> domains;
                for(int i=0; i<vectorLength; i++)
                {
                    domains.push_back(std::make_tuple(0, vectorLength - 1));
                }

                // setup eps
                std::vector<double> eps;
                for(int i=0; i<vectorLength; i++)
                {
                    eps.push_back(1);
                }

                // setup seed
                std::vector<double> seed;
                for(int i=0; i<vectorLength; i++)
                {
                    seed.push_back(i);
                }

                // redirect call
                return toIntTuple(findMin(seed, modF, domains, maxGenerations, eps));
            }

        private:
            // --- methods ---
            static IntTupleType toIntTuple(const TupleType &v)
            {
                IntTupleType intVector;
                for(int i=0; i<v.size(); i++)
                {
                    intVector.push_back(v[i]);
                }
                return intVector;
            }

            static TupleType toPermutationVector(const TupleType &v)
            {
                // asserts
                assert(v.size() > 0);

                // copy original
                std::vector<double> retval(v);

                // ensure monotonicity
                auto changed = true;
                while(changed)
                {
                    changed = false;
                    for(int i=0; i<retval.size(); i++)
                    {
                        for(int j=i+1; j<v.size(); j++)
                            if(retval[j] == retval[i])
                            {
                                retval[j]++;
                                changed = true;
                            }
                    }
                }

                // ensure permutation vector contains one entry for each index
                std::vector<double> mapping(retval);
                std::sort(retval.begin(), retval.end());
                for(int i=0; i<retval.size(); i++)
                {
                    auto pos = std::find(mapping.begin(), mapping.end(), retval[i]) - mapping.begin();
                    retval[i] = pos;
                }

                // return
                return retval;
            }

            // --- members ---
            std::unique_ptr<AbstractMetaHeuristic> m_heuristic;
    };
}

#endif // PERMUTATIONSEARCH_HPP