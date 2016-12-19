#pragma once
#ifndef ADJECENCYLISTGRAPH_HPP
#define ADJECENCYLISTGRAPH_HPP

#include "i2dgraph.hpp"

#include <assert.h>
#include <boost/bimap.hpp>
#include <functional>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <vector>

#include <iostream> // debug

namespace graph
{
    /*! Implementation of IGraph using an adjecency list as the underlying datastructure
     */
    template <typename T>
    class AdjecencyListGraph : public I2DGraph<T>
    {
        public:

            /*! default constructor
             */
            explicit AdjecencyListGraph()
                : m_generator(m_randomDevice())
                , m_distribution(0, std::numeric_limits<ulong>::max())
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
                // insert vertices
                for(auto &vertex : vertices)
                {
                    insertVertex(vertex);
                }
                // insert edges
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
                // insert vertices
                for(auto &vertex : vertices)
                {
                    insertVertex(vertex);
                }
                // insert edges
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
                //std::cout << "insertEdge(" << source << ", " << target << ")" << std::endl;
                assert(hasVertex(source) && hasVertex(target));
                auto sourceId = m_vertexIds.left.find(source)->second;
                auto targetId = m_vertexIds.left.find(target)->second;
                m_edges[sourceId].insert(targetId);
            }

            // --- IGraph ---
            virtual void eraseEdge(const T& source, const T& target) override
            {
                assert(hasVertex(source) && hasVertex(target));
                auto sourceId = m_vertexIds.left.find(source)->second;
                auto targetId = m_vertexIds.left.find(target)->second;
                m_edges[sourceId].erase(targetId);
            }

            // --- IGraph ---
            virtual bool hasEdge(const T& source, const T& target) const override
            {
                if(!hasVertex(source) || !hasVertex(target))
                {
                    return false;
                }
                auto sourceId = m_vertexIds.left.find(source)->second;
                auto targetId = m_vertexIds.left.find(target)->second;
                auto &edgeSet = m_edges.find(sourceId)->second;
                return edgeSet.find(targetId) != edgeSet.end();
            }

            // --- IGraph ---
            virtual const std::set<T> outgoing(const T& source) const override
            {
                //std::cout << "outgoing(" << source << ")" << std::endl;
                assert(hasVertex(source));
                auto sourceId = m_vertexIds.left.find(source)->second;
                std::set<T> retval;
                for(auto targetId : m_edges.find(sourceId)->second)
                {
                    auto &targetVertex = m_vertexIds.right.find(targetId)->second;
                    retval.insert(targetVertex);
                }
                return retval;
            }

            // --- IGraph ---
            virtual const std::set<T> incoming(const T& target) const override
            {
                std::set<T> retval;
                for(auto &source : vertices())
                {
                    if(hasEdge(source, target))
                    {
                        retval.insert(source);
                    }
                }
                return retval;
            }

            // --- IGraph ---
            virtual const std::set<T> vertices() const override
            {
                std::set<T> retval;
                for(auto &tuple : m_vertexIds.left)
                {
                    retval.insert(tuple.first);
                }
                return retval;
            }

            // --- IGraph ---
            virtual void insertVertex(const T& vertex) override
            {
                //std::cout << "insertVertex(" << vertex << ")" << std::endl;
                if(hasVertex(vertex))
                {
                    return;
                }
                auto id = generateNextId();
                using BimapType = typename boost::bimap<T,long>;
                using BimapValueType = typename BimapType::value_type;
                m_vertexIds.insert(BimapValueType(vertex, id));
                m_edges[id] = std::set<long>();
            }

            // --- IGraph ---
            virtual void eraseVertex(const T& vertex) override
            {
                if(!hasVertex(vertex))
                {
                    return;
                }

                // find id
                auto vertexId = m_vertexIds.left.find(vertex)->second;

                // erase from edges (incoming)
                for(auto &tuple : m_edges)
                {
                    tuple.second.erase(vertexId);
                }

                // erase from edges (outgoing)
                m_edges.erase(vertexId);

                // erase points associated to edges using this vertex
                std::vector<std::pair<long,long>> edgesToDelete;
                for(auto &tuple : m_edgePoints)
                {
                    auto &edge = tuple.first;
                    if(edge.first == vertexId || edge.second == vertexId)
                    {
                        edgesToDelete.push_back(edge);
                    }
                }
                for(auto &edge : edgesToDelete)
                {
                    m_edgePoints.erase(edge);
                }

                // erase point associated to this vertex
                m_vertexPoints.erase(vertexId);

                // erase from vertices
                // m_vertexIds.erase(vertexId);

                // #TODO
            }

            // --- IGraph ---
            virtual bool hasVertex(const T &vertex) const override
            {
                //std::cout << "hasVertex(" << vertex << ") = " << (m_vertexIds.left.find(vertex) != m_vertexIds.left.end()) << std::endl;
                return m_vertexIds.left.find(vertex) != m_vertexIds.left.end();
            }

            /*! set the PointType of a vertex
                \param[in] vertex the vertex
                \param[in] point the point (location) of the vertex
             */
            void setVertexPoint(const T& vertex, std::pair<int,int> point)
            {
                assert(hasVertex(vertex));
                auto vertexId = m_vertexIds.left.find(vertex)->second;
                m_vertexPoints[vertexId] = point;
            }

            // --- I2DGraph ---
            virtual const std::pair<int,int> getVertexPoint(const T &vertex) const
            {
                assert(hasVertex(vertex));
                auto vertexId = m_vertexIds.left.find(vertex)->second;
                auto vertexPointIt = m_vertexPoints.find(vertexId);
                assert(vertexPointIt != m_vertexPoints.end());
                return vertexPointIt->second;
            }

            /*! set the path (std::vector<PointType>) for an edge
                \param[in] source the source vertex of the edge
                \param[in] target the target vertex of the edge
                \param[in] path the path (std::vector<PointType>) corresponding to the points the edge passes through
             */
            void setEdgePoints(const T& source, const T& target, std::vector<std::pair<int,int>>& path)
            {
                assert(hasEdge(source, target));
                auto sourceId = m_vertexIds.left.find(source)->second;
                auto targetId = m_vertexIds.left.find(target)->second;
                m_edgePoints[std::make_pair(sourceId, targetId)] = path;
            }

            // --- I2DGraph ---
            virtual std::vector<std::pair<int,int>> const getEdgePoints(const T& source, const T& target) const
            {
                assert(hasEdge(source, target));
                auto sourceId = m_vertexIds.left.find(source)->second;
                auto targetId = m_vertexIds.left.find(target)->second;
                auto edgePointIt = m_edgePoints.find(std::make_pair(sourceId, targetId));
                assert(edgePointIt != m_edgePoints.end());
                return edgePointIt->second;
            }

        private:
            // --- methods ---
            long generateNextId()
            {
                auto nextId = 0L;
                while(m_vertexIds.right.find(nextId) != m_vertexIds.right.end())
                {
                    nextId = m_distribution(m_generator);
                }
                return nextId;
            }

            // --- members ---
            std::random_device                                                      m_randomDevice;
            std::mt19937                                                            m_generator;
            std::uniform_int_distribution<ulong>                                    m_distribution;

            boost::bimap<T, long>                                                   m_vertexIds;
            std::map<long, std::set<long>>                                          m_edges;

            std::map<long, std::pair<int,int>>                                      m_vertexPoints;
            std::map<std::pair<long, long>, std::vector<std::pair<int,int>>>        m_edgePoints;
    };
}

#endif // ADJECENCYLISTGRAPH_HPP