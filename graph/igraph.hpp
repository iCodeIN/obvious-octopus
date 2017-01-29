#pragma once
#ifndef IGRAPH_HPP
#define IGRAPH_HPP

#include <set>
#include <functional>

namespace graph
{
    /*! In mathematics, and more specifically in graph theory, a graph is a representation of a set of objects where some pairs of objects are connected by links. The interconnected objects are represented by mathematical abstractions called vertices (also called nodes or points), and the links         that connect some pairs of vertices are called edges (also called arcs or lines). Typically, a graph is depicted in diagrammatic form as a set of dots for the vertices, joined by lines or curves for the edges. Graphs are one of the objects of study in discrete mathematics.
        The edges may be directed or undirected. For example, if the vertices represent people at a party, and there is an edge between two people if they shake hands, then this is an undirected graph, because if person A shook hands with person B, then person B also shook hands with person A. In   contrast, if there is an edge from person A to person B when person A knows of person B, then this graph is directed, because knowledge of someone is not necessarily a symmetric relation (that is, one person knowing another person does not necessarily imply the reverse; for example, many fans may know of a celebrity, but the celebrity is unlikely to know of all their fans). The former type of graph is called an undirected graph and the edges are called undirected edges while the latter type of graph is called a directed graph and the edges are called directed edges.
        Graphs are the basic subject studied by graph theory. The word "graph" was first used in this sense by J. J. Sylvester in 1878.
     */
    template <typename T>
    class IGraph
    {
        public:
            /* default constructor
             */
            explicit IGraph() = default;

            /* default destructor
             */
            virtual ~IGraph() = default;

            /*! Prohibit const copy constructor
             */
            IGraph(const IGraph&) = delete;

            /*! Prohibit copy constructor
             */
            IGraph(IGraph&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const IGraph&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(IGraph&) = delete;

            /*! Insert an edge into the graph
                \param[in] source the source vertex
                \param[in] target the target vertex
             */
            virtual void insertEdge(const T& source, const T& target) = 0;

            /*! Erase an edge from the graph
                \param[in] source the source vertex
                \param[in] target the target vertex
             */
            virtual void eraseEdge(const T& source, const T& target) = 0;

            /*! \return whether this graph has an edge specified by to vertices
                \param[in] source the source vertex
                \param[in] target the target vertex
             */
            virtual bool hasEdge(const T& source, const T& target) const = 0;

            /*! \return a set of the vertices connected to a given vertex by outgoing edges
             */
            virtual const std::set<T> outgoing(const T& source) const = 0;

            /*! \return a set of the vertices connected to a given vertex by incoming edges
             */
            virtual const std::set<T> incoming(const T& target) const = 0;

            /*! \return a set of all vertices in this graph
             */
            virtual const std::set<T> vertices() const = 0;

            /*! Insert a vertex into the graph
                \param[in] vertex the vertex to be inserted
             */
            virtual void insertVertex(const T& vertex) = 0;

            /*! Erase a vertex from the graph
                \param[in] vertex the vertex to be erased
             */
            virtual void eraseVertex(const T& vertex) = 0;

            /*! \return true if this graph has a given vertex, false otherwise
             */
            virtual bool hasVertex(const T& vertex) const = 0;

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // IGRAPH_HPP