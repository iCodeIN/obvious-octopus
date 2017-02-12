#pragma once
#ifndef META_ASSIGNMENTSEARCH_HPP
#define META_ASSIGNMENTSEARCH_HPP

#include "meta/abstractmetaheuristic.hpp"

#include <memory>

namespace meta
{
    /*! This class acts as a wrapper around AbstractMetaHeuristic. It provides specific functionality for solving assignment problems.
        A problem is considered a permutation problem iff a number (S) of sources is to be mapped unto a number of targets (T) such that given conditions are met.
        This class will use the wrapped AbstractMetaHeuristic (and some utility methods for conversion) to solve the problem.
     */
    class AssignmentSearch final : public AbstractMetaHeuristic
    {
        public:

            /*! constructor
                \param[in] heuristic the wrapped heuristic
             */
            explicit AssignmentSearch(std::unique_ptr<AbstractMetaHeuristic> heuristic)
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
            using IntFunctionType = std::function<double(int sourceNr, int targetNr)>;

            /*! TODO
             */
            IntTupleType findIntMax(const IntFunctionType &function, int nofSources, int nofTargets, int maxGenerations)
            {
                assert(nofSources >= nofTargets);

                // function transform
                auto modF = [this, &function, &nofTargets](const TupleType &t)
                {
                    auto  v = toIntTuple(t);
                    double retval = 0;
                    for(int i=0; i<v.size(); i++)
                    {
                        retval += function(i, v[i] >= nofTargets ? -1 : v[i]);
                    }
                    return retval;
                };

                // setup domains
                std::vector<DomainType> domains;
                for(int i=0; i < nofSources; i++)
                {
                    domains.push_back(std::make_tuple(0, nofTargets - 1));
                }

                // setup eps
                std::vector<double> eps;
                for(int i=0; i<nofSources; i++)
                {
                    eps.push_back(1);
                }

                // setup seed
                std::vector<double> seed;
                for(int i=0; i<nofSources; i++)
                {
                    seed.push_back(i);
                }

                // redirect call
                return toIntTuple(findMax(seed, modF, domains, maxGenerations, eps));
            }

            /*! TODO
             */
            IntTupleType findIntMin(const IntFunctionType &function, int nofSources, int nofTargets, int maxGenerations)
            {
                assert(nofSources >= nofTargets);

                // function transform
                auto modF = [this, &function, &nofTargets](const TupleType &t)
                {
                    auto  v = toIntTuple(t);
                    double retval = 0;
                    for(int i=0; i<v.size(); i++)
                    {
                        retval += function(i, v[i] >= nofTargets ? -1 : v[i]);
                    }
                    return retval;
                };

                // setup domains
                std::vector<DomainType> domains;
                for(int i=0; i<nofSources; i++)
                {
                    domains.push_back(std::make_tuple(0, nofTargets - 1));
                }

                // setup eps
                std::vector<double> eps;
                for(int i=0; i<nofSources; i++)
                {
                    eps.push_back(1);
                }

                // setup seed
                std::vector<double> seed;
                for(int i=0; i<nofSources; i++)
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

                // ensure uniqueness
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

                // map to valid index
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

#endif // META_ASSIGNMENTSEARCH_HPP