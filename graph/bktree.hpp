#ifndef BKTREE_HPP
#define BKTREE_HPP

#include "graph/adjecencylisttree.hpp"

namespace graph
{
    template <typename T>
    class BKTree : public AdjecencyListTree<T>
    {
        public:
            using MetricFunctionType = std::function<int(const T& t0, const T& t1)>;

            explicit BKTree(const MetricFunctionType& metric)
                : m_metric(metric)
            {
            }

            /*!
             */
            void insert(const T& t)
            {

            }
            /*!
             */
            void find(const T& t, int distance)
            {

            }

        private:
            // --- methods ---
            // --- members ---
            MetricFunctionType m_metric;
    };
}

#endif // BKTREE_HPP