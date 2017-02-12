#pragma once
#ifndef XML_XML_HPP
#define XML_XML_HPP

#include "xml/defaultelementimpl.hpp"
#include "xml/ielement.hpp"

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <memory>
#include <stack>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

/*! Writes an Element to an std::ostream
     	 \param[in] os	std::ostream to write the XML::Element to
    	 \param[in] e	XML::Element to stream
     */
static std::ostream& operator<<(std::ostream& os, XML::IElement &e)
{
    using BoostTreeType = boost::property_tree::ptree;
    using StackType = std::pair<std::string, const XML::IElement*>;

    // use boost to persist the IElement
    BoostTreeType tree;

    // use stack to recursively build the tree
    std::stack<StackType> stk;
    stk.push(std::make_pair(e.getName(), &e));

    // main loop
    while(!stk.empty())
    {
        // pop top element
        auto top = stk.top();
        stk.pop();

        auto &pt = top.first;
        auto &el = top.second;

        // text
        auto &it = tree.add(pt, el->getText());

        // attributes
        for(auto pair : el->getAttributes())
        {
            it.add("<xmlattr>." + pair.first, pair.second);
        }

        /*  go over children
            this needs to be done backwards, since the approach with the stack will reverse the order of nodes
        */
        for(int i=el->countChildren() - 1; i >= 0; i--)
        {
            auto childPtr = &(el->getChild(i));
            stk.push(std::make_pair(pt + "." + childPtr->getName(), childPtr));
        }
    }

    // write
    boost::property_tree::write_xml(os, tree, boost::property_tree::xml_writer_make_settings<std::string>('	', 1));

    // return
    return os;
}

/*! Reads an Element from an std::istream
 	 \param[in] is	std::istream to read the XML::Element from
	 \param[in] e	XML::Element to stream
 */
static std::istream& operator>>(std::istream& is, XML::IElement &e)
{
    using BoostTreeType = boost::property_tree::ptree;
    using StackType = std::pair<const BoostTreeType*, XML::IElement*>;

    // use boost to read the xml inputstream
    BoostTreeType tree;
    boost::property_tree::read_xml(is, tree);

    // use stack to recursively read the tree
    std::stack<StackType> stk;

    // find root (discarding comment nodes)
    for(auto &child : tree)
    {
        if(child.first != "<xmlcomment>")
        {
            e.setName(child.first);
            stk.push(std::make_pair(&child.second, &e));
        }
    }
    assert(stk.size() <= 1);

    // main loop
    while(!stk.empty())
    {
        // pop top element
        auto top = stk.top();
        stk.pop();

        // separate different trees
        auto &tr = top.first;           //!< boost tree
        auto &el = top.second;          //!< IElement
        assert(tr);
        assert(el);

        // set text (discard text only consisting of whitespaces)
        auto text = tr->get_value<std::string>();
        if(!text.empty() && std::find_if(text.begin(), text.end(), [](const char c)
    {
        return !isspace(c);
        })!=text.end())
        {
            el->setText(text);
        }

        // go over children
        for(auto &child : *tr)
        {
            auto &childName = child.first;
            if(childName == "<xmlattr>")
            {
                for(auto attrChild : child.second)
                {
                    el->setAttribute(attrChild.first, attrChild.second.get_value<std::string>());
                }
            }
            else
            {
                auto childTrPtr = &child.second;
                auto childElUPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(childName));
                auto childElPtr  = childElUPtr.get();
                el->add(std::move(childElUPtr));
                stk.push(std::make_pair(childTrPtr, childElPtr));
            }
        }

    }

    // return inputstream
    return is;
}

#endif // XML_XML_HPP