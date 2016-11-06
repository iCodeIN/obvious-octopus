#pragma once
#ifndef ADJECENCYLISTTREE_HPP
#define ADJECENCYLISTTREE_HPP

#include "adjecencylistgraph.hpp"
#include "itree.hpp"

#include <assert.h>
#include <memory>
#include <set>

namespace graph
{
    /*!
     */
    template <typename T>
    class AdjecencyListTree : public ITree<T>
    {
        public:
            /*! default constructor
             */
            explicit AdjecencyListTree()
                : m_graph(std::unique_ptr<IGraph<T>>(new AdjecencyListGraph<T>()))
            {
            }

            /*! default destructor
             */
            virtual ~AdjecencyListTree() = default;

            // --- IGraph ---
            virtual void insertEdge(const T& source, const T& target)
            {
                assert(m_graph->incoming(target).size() == 0);
                assert(source != target);

                m_graph->insertEdge(source, target);
            }

            // --- IGraph ---
            virtual void eraseEdge(const T& source, const T& target)
            {
                m_graph->eraseEdge(source, target);
            }

            // --- IGraph ---
            virtual bool hasEdge(const T& source, const T& target) const
            {
                return m_graph->hasEdge(source, target);
            }

            // --- IGraph ---
            virtual const std::set<T> outgoing(const T& source) const
            {
                return m_graph->outgoing(source);
            }

            // --- IGraph ---
            virtual const std::set<T> incoming(const T& target) const
            {
                return m_graph->incoming(target);
            }

            // --- IGraph ---
            virtual const std::set<T> vertices() const
            {
                return m_graph->vertices();
            }

            // --- IGraph ---
            virtual bool hasVertex(const T& vertex) const
            {
                return m_graph->hasVertex(vertex);
            }

            // --- ITree ---
            virtual const std::set<T> leaves() const override
            {
                std::set<T> l;
                for(auto &vertex : vertices())
                {
                    if(outgoing(vertex).empty())
                    {
                        l.insert(vertex);
                    }
                }
                return l;
            }

            // --- ITree ---
            virtual const T root() const override
            {
                std::set<T> r;
                for(auto &vertex : vertices())
                {
                    if(incoming(vertex).size() == 0)
                    {
                        r.insert(vertex);
                    }
                }
                assert(r.size() == 1);
                return *(r.begin());
            }

            // --- ITree ---
            virtual int depth() const override
            {
                return depthAt(root());
            }

        private:
            // --- methods ---
            int depthAt(const T& vertex) const
            {
                if(outgoing(vertex).empty())
                {
                    return 0;
                }
                else
                {
                    auto maxDepth = 0;
                    for(auto &child : outgoing(vertex))
                    {
                        maxDepth = std::max(maxDepth, depthAt(child));
                    }
                    return maxDepth + 1;
                }
            }
            // --- members ---
            std::unique_ptr<IGraph<T>>  m_graph;
            std::set<T>                 m_leaves;
            std::set<T>                 m_roots;
    };
}

#endif // ADJECENCYLISTTREE_HPP