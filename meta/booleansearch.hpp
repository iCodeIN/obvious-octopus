#pragma once
#ifndef META_BOOLEANSEARCH_HPP
#define META_BOOLEANSEARCH_HPP

#include "meta/abstractmetaheuristic.hpp"

#include <memory>

namespace meta
{
    /*! This class acts as a wrapper around AbstractMetaHeuristic. It provides specific functionality for solving boolean satisfiability problems.
        A problem is considered a boolean satisfiability problem iff all input variables are boolean.
     */
    class BooleanSearch final : public AbstractMetaHeuristic
    {
        public:

            /*! constructor
                \param[in] heuristic the wrapped heuristic
             */
            explicit BooleanSearch(std::unique_ptr<AbstractMetaHeuristic> heuristic)
                : m_heuristic(std::move(heuristic))
            {
            }

            // --- AbstractMetaHeuristic ---
            virtual TupleType findMax(const TupleType &seed, const FunctionType &function, const std::vector<DomainType> &domains, int maxGenerations, std::vector<double> eps)
            {
                return m_heuristic.get()->findMax(seed, modF, domains, maxGenerations, eps);
            }

            /*! Define the input type for this kind of metaheuristic
             */
            using BoolTupleType = std::vector<bool>;

            /*! Define the type of the function to be optimized for this kind of metaheuristic
             */
            using BoolFunctionType = std::function<double(const std::vector<bool>& bits)>;

            /*! TODO
             */
            IntTupleType findBoolMax(const BoolFunctionType &function, int nofBits, int maxGenerations)
            {
                // function transform
                auto modF = [this, &function](const TupleType &t)
                {
                    std::vector<bool> t1;
                    for(int i=0; i<t.size(); i++)
                    {
                        if(t[i] >= 0.5)
                        {
                            t1.push_back(true);
                        }
                        else
                        {
                            t1.push_back(false);
                        }
                    }
                    return function(t1);
                };

                // setup domains
                std::vector<DomainType> domains;
                for(int i=0; i < nofSources; i++)
                {
                    domains.push_back(std::make_tuple(0, 1));
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
                    seed.push_back(0);
                }

                // redirect call
                return toBoolTuple(findMax(seed, modF, domains, maxGenerations, eps));
            }

            /*! TODO
             */
            IntTupleType findIntMin(const IntFunctionType &function, int nofSources, int nofTargets, int maxGenerations)
            {
                // function transform
                auto modF = [this, &function](const TupleType &t)
                {
                    std::vector<bool> t1;
                    for(int i=0; i<t.size(); i++)
                    {
                        if(t[i] >= 0.5)
                        {
                            t1.push_back(true);
                        }
                        else
                        {
                            t1.push_back(false);
                        }
                    }
                    return function(t1);
                };

                // setup domains
                std::vector<DomainType> domains;
                for(int i=0; i < nofSources; i++)
                {
                    domains.push_back(std::make_tuple(0, 1));
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
                    seed.push_back(0);
                }

                // redirect call
                return toBoolTuple(findMin(seed, modF, domains, maxGenerations, eps));
            }

        private:
            // --- methods ---
            static BoolTupleType toBoolTuple(const TupleType &v)
            {
                BoolTupleType retval;
                for(int i=0; i<v.size(); i++)
                {
                    retval.push_back(v[i]) >= 0.5);
                }
                return retval;
            }

            // --- members ---
            std::unique_ptr<AbstractMetaHeuristic> m_heuristic;
    };
}

#endif // META_BOOLEANSEARCH_HPP