#pragma once
#ifndef CIRCULARGRAPHLAYOUT_HPP
#define CIRCULARGRAPHLAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"

namespace graphUI
{
    /*! In graph drawing, a circular layout is a style of drawing that places the vertices of a graph on a circle, often evenly spaced so that they form the vertices of a regular polygon.
        Circular layouts are a good fit for communications network topologies such as star or ring networks, and for the cyclic parts of metabolic networks.
        For graphs with a known Hamiltonian cycle, a circular layout allows the cycle to be depicted as the circle,
        and in this way circular layouts form the basis of the LCF notation for Hamiltonian cubic graphs.
        A circular layout may be used on its own for an entire graph drawing, but it also may be used as the layout for smaller clusters of vertices within a larger graph drawing,
        such as its biconnected components, clusters of genes in a gene interaction graph, or natural subgroups within a social network.
        If multiple vertex circles are used in this way, other methods such as force-directed graph drawing may be used to arrange the clusters.
        One advantage of a circular layout in some of these applications, such as bioinformatics or social network visualization, is its neutrality:
        by placing all vertices at equal distances from each other and from the center of the drawing, none is given a privileged position,
        countering the tendency of viewers to perceive more centrally located nodes as being more important.
     */
    template <typename T>
    class CircularGraphLayout final : public AbstractGraphLayout<T>
    {
        public:
            explicit CircularGraphLayout() = default;
            virtual ~CircularGraphLayout() = default;

            // --- AbstractGraphLayout ---
            virtual typename graphUI::AbstractGraphLayout<T>::LayoutType layout(const graph::Graph<T> &graph, const typename graphUI::AbstractGraphLayout<T>::SizeFunctionType &size) override
            {
            }

        private:
            // --- methods ---
            std::vector<T> relativeLayout(graph::Graph<T> &graph)
            {
                // vertices
                std::vector<T> vertices;
                std::for_each(graph.vertices().begin(), graph.vertices().end(), [&vertices](const T &vertex)
                {
                    vertices.push_back(vertex);
                });

                // count nr of crossings
                auto f = [&vertices](const std::vector<int> &input)
                {
                    for(int i=0; i<vertices.size(); i++)
                    {
                        auto sourceVertex = vertices[input[i]];
                        for(auto &out : graph.outgoing(sourceVertex))
                        {
                            // lookup outgoing index
                        }
                    }
                };

                /* run the metaheuristic
                 */
                auto innerSearch = std::unique_ptr<meta::AbstractMetaHeuristic>(new meta::TabuSearch());
                auto search = meta::PermutationSearch(std::move(innerSearch));
                auto bestPermutation = search.findIntMin(f, vertices.size(), 1000);
            }
    };
}
#endif // CIRCULARGRAPHLAYOUT_HPP