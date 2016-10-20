#pragma once
#ifndef ADJECENCYLISTGRAPH_HPP
#define ADJECENCYLISTGRAPH_HPP

#include "igraph.hpp"

#include <assert.h>
#include <functional>
#include <map>
#include <set>

namespace graph
{
    /*! Implementation of IGraph using an adjecency list as the underlying datastructure
     */
    template <typename T>
    class AdjecencyListGraph : public IGraph<T>
    {
        public:

            /*! default constructor
             */
            explicit AdjecencyListGraph()
            {
            }

            /*! transfer function definition
                \param[in] source source vertex
                \param[in] target target vertex
                \return true iff a vertex from source to target exists, false otherwise
             */
            using TransferFunctionType0 = std::function<bool(const T &source, const T &target)>;

            /*! transfer function definition
                \param[in] source source vertex
                \return a set of vertices that can be reached from the given source vertex
             */
            using TransferFunctionType1 = std::function<std::set<T>(const T &source)>;

            /*! constructor
                \param[in] vertices         the vertices in the graph
                \param[in] transferFunction the transferFunction to be used
             */
            explicit AdjecencyListGraph(std::set<T> vertices, const TransferFunctionType0 &transferFunction)
            {
                for(auto &vertex0 : vertices)
                {
                    for(auto &vertex1 : vertices)
                    {
                        if(transferFunction(vertex0, vertex1))
                        {
                            insertEdge(vertex0, vertex1);
                        }
                    }
                }
            }

            /*! constructor
                \param[in] vertices         the vertices in the graph
                \param[in] transferFunction the transferFunction to be used
             */
            explicit AdjecencyListGraph(std::set<T> vertices, const TransferFunctionType1 &transferFunction)
            {
                for(auto &vertex0 : vertices)
                {
                    for(auto &vertex1 : transferFunction(vertex0))
                    {
                        insertEdge(vertex0, vertex1);
                    }
                }
            }

            // --- IGraph ---
            virtual void insertEdge(const T& source, const T& target) override
            {
                m_vertices[source].insert(target);
            }

            // --- IGraph ---
            virtual void eraseEdge(const T& source, const T& target) override
            {
                m_vertices[source].erase(target);
            }

            // --- IGraph ---
            bool hasEdge(const T& source, const T& target) const override
            {
                if(!hasVertex(source) || !hasVertex(target))
                {
                    return false;
                }
                auto adjSetIt = m_vertices.find(source);
                if(adjSetIt == m_vertices.cend())
                {
                    return false;
                }
                auto &adjSet = adjSetIt->second;
                return adjSet.find(target) != adjSet.cend();
            }

            // --- IGraph ---
            virtual const std::set<T> outgoing(const T& source) const override
            {
                auto adjSetIt = m_vertices.find(source);
                if(adjSetIt != m_vertices.cend())
                {
                    return adjSetIt->second;
                }
                else
                {
                    return std::set<T>();
                }
            }

            // --- IGraph ---
            virtual const std::set<T> incoming(const T& target) const override
            {
                auto retSet = std::set<T>();
                for(auto &pair : m_vertices)
                {
                    auto &nbs = pair.second;
                    if(nbs.find(target) != nbs.cend())
                    {
                        retSet.insert(pair.first);
                    }
                }
                return retSet;
            }

            // --- IGraph ---
            virtual const std::set<T> vertices() const override
            {
                auto retSet = std::set<T>();
                for(auto &pair : m_vertices)
                {
                    retSet.insert(pair.first);
                    for(auto &nb : pair.second)
                    {
                        retSet.insert(nb);
                    }
                }
                return retSet;
            }

            // --- IGraph ---
            virtual bool hasVertex(const T &vertex) const override
            {
                if(m_vertices.find(vertex) != m_vertices.cend())
                {
                    return true;
                }
                for(auto &pair : m_vertices)
                {
                    auto &nbs = pair.second;
                    if(nbs.find(vertex) != nbs.cend())
                    {
                        return true;
                    }
                }
                // default
                return false;
            }

        private:
            // --- methods ---
            // --- members ---
            std::map<T, std::set<T>>   m_vertices;
    };
}

#endif // ADJECENCYLISTGRAPH_HPP