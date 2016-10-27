#pragma once
#ifndef bGRAPHML_HPP
#define bGRAPHML_HPP

#include "graph/igraph.hpp"
#include "graph/adjecencylistgraph.hpp"

#include "xml/boostelementimpl.hpp"

#include <assert.h>
#include <istream>
#include <ostream>
#include <string>

namespace XML
{
    /*!
     */
    static std::ostream& operator<<(std::ostream& os, const graph::IGraph<std::string>* g)
    {
        // build xml
        // persist xml
        // return
        return os;
    }

    static std::istream& operator>>(std::istream& is, graph::IGraph<std::string>* graphPtr)
    {

        // read element
        auto elementPtr = new BoostElementImpl("");
        is >> *elementPtr;

        // build graph
        assert(elementPtr->hasChildByName("graph"));
        auto &graphElement = elementPtr->getChildByName("graph");

        // determine edge (default) orientation
        assert(graphElement.hasAttribute("edgedefault"));
        assert(graphElement.getAttribute("edgedefault") == "directed" || graphElement.getAttribute("edgedefault") == "undirected");
        auto isGraphDirected = graphElement.getAttribute("edgedefault") == "directed";

        // vertices
        assert(graphPtr != nullptr);
        for(auto vertexElement : graphElement.getChildrenByName("node"))
        {
        }

        // edges
        for(auto edgeElement : graphElement.getChildrenByName("edge"))
        {
            assert(edgeElement.get().hasAttribute("source") && edgeElement.get().hasAttribute("target"));
            auto source = edgeElement.get().getAttribute("source");
            auto target = edgeElement.get().getAttribute("target");
            if(isGraphDirected)
            {
                // add edge
                graphPtr->insertEdge(source, target);
            }
            else
            {
                // add edge
                graphPtr->insertEdge(source, target);
                // add reverse edge
                graphPtr->insertEdge(target, source);
            }
        }

        // return
        return is;
    }
}

#endif // bGRAPHML_HPP