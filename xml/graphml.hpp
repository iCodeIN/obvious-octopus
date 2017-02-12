#pragma once
#ifndef XML_GRAPHML_HPP
#define XML_GRAPHML_HPP

#include "graph/igraph.hpp"
#include "graph/adjecencylistgraph.hpp"

#include "xml/xml.hpp"

#include <assert.h>
#include <istream>
#include <ostream>
#include <string>

/*!
 */
static std::ostream& operator<<(std::ostream& os, const graph::IGraph<std::string> &g)
{
    // build xml
    auto rootElementPtr = new XML::DefaultElementImpl("graphml");

    auto graphElementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl("graph"));
    graphElementPtr->setAttribute("id", "G");
    graphElementPtr->setAttribute("edgedefault", "directed");

    for(auto &vertex : g.vertices())
    {
        auto vertexElementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl("node"));
        vertexElementPtr->setAttribute("id", vertex);
        graphElementPtr->add(std::move(vertexElementPtr));
    }
    auto edgeId = 0;
    for(auto &vertex0 : g.vertices())
    {
        for(auto &vertex1 : g.vertices())
        {
            if(g.hasEdge(vertex0, vertex1))
            {
                auto edgeElementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl("edge"));
                edgeElementPtr->setAttribute("id", edgeId);
                edgeElementPtr->setAttribute("source", vertex0);
                edgeElementPtr->setAttribute("target", vertex1);
                graphElementPtr->add(std::move(edgeElementPtr));
                edgeId++;
            }
        }
    }

    // add graph element to root element
    rootElementPtr->add(std::move(graphElementPtr));

    // persist xml
    os << *rootElementPtr;

    // return
    return os;
}

/*!
 */
static std::istream& operator>>(std::istream& is, graph::IGraph<std::string> &g)
{

    // read element
    auto elementPtr = new XML::DefaultElementImpl("");
    is >> *elementPtr;

    // build graph
    assert(elementPtr->hasChildByName("graph"));
    auto &graphElement = elementPtr->getChildByName("graph");

    // determine edge (default) orientation
    assert(graphElement.hasAttribute("edgedefault"));
    assert(graphElement.getAttribute("edgedefault") == "directed" || graphElement.getAttribute("edgedefault") == "undirected");
    auto isGraphDirected = graphElement.getAttribute("edgedefault") == "directed";

    // vertices
    for(auto vertexElement : graphElement.getChildrenByName("node"))
    {
        auto &id = vertexElement.get().getAttribute("id");
        g.insertVertex(id);
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
            g.insertEdge(source, target);
        }
        else
        {
            // add edge
            g.insertEdge(source, target);
            // add reverse edge
            g.insertEdge(target, source);
        }
    }

    // return
    return is;
}

#endif // XML_GRAPHML_HPP