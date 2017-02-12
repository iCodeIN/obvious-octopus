#pragma once
#ifndef GRAPH_ADJECENCYLISTTREE_HPP
#define GRAPH_ADJECENCYLISTTREE_HPP

#include "graph/adjecencylistgraph.hpp"
#include "graph/itree.hpp"

#include <assert.h>
#include <memory>
#include <set>

namespace graph
{
    /*! Implementation of ITree using AdjecencyListGraph as underlying datastructure
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

            /*! Prohibit const copy constructor
             */
            AdjecencyListTree(const AdjecencyListTree&) = delete;

            /*! Prohibit copy constructor
             */
            AdjecencyListTree(AdjecencyListTree&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const AdjecencyListTree&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(AdjecencyListTree&) = delete;

            //! --- IGraph ---
            virtual void insertEdge(const T& source, const T& target) override
            {
                assert(m_graph->incoming(target).size() == 0);
                assert(source != target);

                m_graph->insertEdge(source, target);
            }

            //! --- IGraph ---
            virtual void eraseEdge(const T& source, const T& target) override
            {
                m_graph->eraseEdge(source, target);
            }

            //! --- IGraph ---
            virtual bool hasEdge(const T& source, const T& target) const override
            {
                return m_graph->hasEdge(source, target);
            }

            //! --- IGraph ---
            virtual const std::set<T> outgoing(const T& source) const override
            {
                return m_graph->outgoing(source);
            }

            //! --- IGraph ---
            virtual const std::set<T> incoming(const T& target) const override
            {
                return m_graph->incoming(target);
            }

            //! --- IGraph ---
            virtual const std::set<T> vertices() const override
            {
                return m_graph->vertices();
            }

            //! --- IGraph ---
            virtual void insertVertex(const T& vertex) override
            {
                m_graph->insertVertex(vertex);
            }

            //! --- IGraph ---
            virtual void eraseVertex(const T& vertex) override
            {
                m_graph->eraseVertex(vertex);
            }

            //! --- IGraph ---
            virtual bool hasVertex(const T& vertex) const override
            {
                return m_graph->hasVertex(vertex);
            }

            //! --- ITree ---
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

            //! --- ITree ---
            virtual T parent(const T& vertex) const override
            {
                auto &in = incoming(vertex);
                return *in.begin();
            }

            //! --- ITree ---
            virtual T root() const override
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

            //! --- ITree ---
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

#endif // GRAPH_ADJECENCYLISTTREE_HPP