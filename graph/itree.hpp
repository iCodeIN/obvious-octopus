#pragma once
#ifndef ITREE_HPP
#define ITREE_HPP

#include "graph/igraph.hpp"

#include <set>

namespace graph
{
    /*! In computer science, a tree is a widely used abstract data type (ADT)—or data structure implementing this ADT—that simulates a hierarchical tree structure, with a root value and subtrees of children with a parent node, represented as a set of linked nodes.
        A tree data structure can be defined recursively (locally) as a collection of nodes (starting at a root node), where each node is a data structure consisting of a value, together with a list of references to nodes (the "children"), with the constraints that no reference is duplicated, and node points to the root.
        Alternatively, a tree can be defined abstractly as a whole (globally) as an ordered tree, with a value assigned to each node. Both these perspectives are useful: while a tree can be analyzed mathematically as a whole, when actually represented as a data structure it is usually represented and worked with separately by node (rather than as a list of nodes and an adjacency list of edges between nodes, as one may represent a digraph, for instance). For example, looking at a tree as a whole, one can talk about "the parent node" of a given node, but in general as a data structure a given node only contains the list of its children, but does not contain a reference to its parent (if any).
     */
    template <typename T>
    class ITree : public IGraph<T>
    {
        public:
            /*! default constructor
             */
            explicit ITree() = default;

            /*! default destructor
             */
            virtual ~ITree() = default;

            /*! \return the leaves of the ITree
             */
            virtual const std::set<T> leaves() const = 0;

            /*! \return the parent of a given vertex
             */
            virtual T parent(const T& vertex) const = 0;

            /*! \return the root of the ITree
             */
            virtual T root() const = 0;

            /*! \return the depth of the ITree
             */
            virtual int depth() const = 0;
    };
}

#endif // ITREE_HPP