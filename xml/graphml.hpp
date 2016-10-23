#pragma once
#ifndef GRAPHML_HPP
#define GRAPHML_HPP

#include "graph/adjecencylistgraph.hpp"

namespace XML
{
    class GraphML final
    {
        public:
            /*!
             */
            friend std::ostream& operator<<(std::ostream& os, const graph::IGraph* g)
            {
                // build xml
                // persist xml
                // return
                return os;
            }

            friend std::istream& operator>>(std::istream& is, graph::IGraph* e)
            {
                // read element
                // build graph
                // return
                return is;
            }

        private:
            // --- methods ---
            // --- members ---
    };
}

#endif // GRAPHML_HPP