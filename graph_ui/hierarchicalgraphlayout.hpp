#pragma once
#ifndef HIERARCHICALGRAPHLAYOUT_HPP
#define HIERARCHICALGRAPHLAYOUT_HPP

#include "graph_ui/abstractgraphlayout.hpp"
#include "graph/bfs.hpp"
#include "graph/igraph.hpp"
#include "graph/adjecencylistgraph.hpp"

#include <algorithm>
#include <assert.h>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <tuple>

#include "meta/tabusearch.hpp"
#include "meta/permutationsearch.hpp"

#include <iostream> // DEBUG

namespace graph
{
    /*! Layered graph drawing or hierarchical graph drawing is a type of graph drawing in which the vertices of a directed graph are drawn in horizontal rows or layers with the edges generally directed downwards.
        It is also known as Sugiyama-style graph drawing after Kozo Sugiyama, who first developed this drawing style.
        The ideal form for a layered drawing would be an upward planar drawing, in which all edges are oriented in a consistent direction and no pairs of edges cross. However, graphs often contain cycles, minimizing the number of inconsistently-oriented edges is NP-hard, and minimizing the number of crossings is also NP-hard, so layered graph drawing systems typically apply a sequence of heuristics that reduce these types of flaws in the drawing without guaranteeing to find a drawing with the minimum number of flaws.
     */
    template <typename T>
    class HierarchicalGraphLayout final : public graphUI::AbstractGraphLayout<T>
    {
        public:

            /*! default constructor
             */
            explicit HierarchicalGraphLayout() = default;

            /*! default destructor
             */
            virtual ~HierarchicalGraphLayout() = default;

            // --- AbstractGraphLayout ---
            virtual typename graphUI::AbstractGraphLayout<T>::LayoutType layout(const graph::IGraph<T> &graph, const typename graphUI::AbstractGraphLayout<T>::SizeFunctionType &sizeFunction) override
            {

                // determine layers
                auto layers = determineLayers(graph);

                // setup graph with dummy nodes
                AdjecencyListGraph<long> graphWithDummies;
                auto dummyInformation = setupDummyGraph(layers, graph, graphWithDummies);
                auto &vertexMapping          = std::get<0>(dummyInformation);
                auto &graphWithDummiesLayers = std::get<1>(dummyInformation);
                auto &graphPathsToDummies    = std::get<2>(dummyInformation);

                // DEBUG
                for(auto &pair : vertexMapping)
                {
                    std::cout << pair.first << "\t:\t" << pair.second << std::endl;
                }

                // determine relative layout for each layer
                auto relLayout = relativeLayout(graphWithDummiesLayers, graphWithDummies);

                // DEBUG
                std::cout << "RELATIVE LAYOUT" << std::endl;
                std::cout << "---------------" << std::endl;
                std::cout << std::endl;
                for(auto &pair : relLayout)
                {
                    std::cout << pair.first << " : ";
                    for(auto &vertex : pair.second)
                    {
                        std::cout << vertex << "\t";
                    }
                    std::cout << std::endl;
                }

                // determine absolute layout
                auto absLayout = absoluteLayout(relLayout, vertexMapping, sizeFunction);

                // DEBUG
                std::cout << "ABSOLUTE LAYOUT" << std::endl;
                std::cout << "---------------" << std::endl;
                std::cout << std::endl;
                for(auto &pair : vertexMapping)
                {
                    std::cout << pair.first << " : " << std::get<0>(absLayout[pair.second]) << "," << std::get<1>(absLayout[pair.second]) << std::endl;
                }

                // create VertexLayoutFunction
                auto vF = [&graph, absLayout, vertexMapping](const T &vertex)
                {
                    assert(graph.hasVertex(vertex));
                    assert(vertexMapping.find(vertex) != vertexMapping.end());
                    assert(absLayout.find(vertexMapping.find(vertex)->second) != absLayout.end());
                    return absLayout.find(vertexMapping.find(vertex)->second)->second;
                };

                // create EdgeLayoutFunction
                auto eF = [&graph, absLayout, graphPathsToDummies, vertexMapping](const T &source, const T &target)
                {
                    assert(graph.hasEdge(source, target));
                    auto dummySource = vertexMapping.find(source)->second;
                    auto dummyTarget = vertexMapping.find(target)->second;

                    // create path
                    std::vector<std::tuple<int,int>> path;
                    if(graphPathsToDummies.find(std::make_tuple(dummySource, dummyTarget)) == graphPathsToDummies.end())
                    {
                        path.push_back(absLayout.find(dummySource)->second);
                        path.push_back(absLayout.find(dummyTarget)->second);
                    }
                    else
                    {
                        auto &dummyPath = graphPathsToDummies.find(std::make_tuple(dummySource, dummyTarget))->second;
                        for(auto &id : dummyPath)
                        {
                            path.push_back(absLayout.find(id)->second);
                        }
                    }

                    // return
                    return path;
                };

                // return
                return std::make_tuple(vF, eF);
            }

        private:
            // --- methods ---

            /*! Organize all nodes of the graph into layers.
                This method will generate a second graph, possibly containing dummy-vertices
                in order to fulfill the criteria set by the remainder of the algorithm.
                \param[in] graph    the graph being processed
             */
            std::map<T, int> determineLayers(const IGraph<T> &graph) const
            {
                std::map<T, int> layers;
                auto &vertices = graph.vertices();
                std::for_each(vertices.begin(), vertices.end(), [this, &graph, &layers](const T &vertex)
                {
                    layers[vertex] = graph.outgoing(vertex).empty() ? 0 : bfs<T>(graph, vertex)->depth();
                });
                return layers;
            }

            /*! Setup the graph with dummy nodes
                \param[in] layerInformation     layer information for each node in the original graph
                \param[in] graph                original graph
                \param[in] graphWithDummies     graph with dummy nodes
             */
            using VertexMapType = std::map<T, long>;
            using LayerMapType = std::map<long, int>;
            using DummyPathMapType = std::map<std::tuple<long,long>, std::vector<long>>;
            std::tuple<VertexMapType, LayerMapType, DummyPathMapType> setupDummyGraph(const std::map<T,int> layerInformation, const IGraph<T> &graph, IGraph<long> &graphWithDummies) const
            {
                // setup structures for return values
                VertexMapType         vertexMap;
                LayerMapType          layerMap;
                DummyPathMapType      dummyPathMap;

                // fill in what we know
                auto vertices = graph.vertices();
                std::for_each(vertices.begin(), vertices.end(), [this, &vertexMap, &layerMap, &graphWithDummies, &layerInformation](const T &vertex)
                {
                    auto id = vertexMap.size();
                    vertexMap[vertex] = id;
                    layerMap[id] = layerInformation.find(vertex)->second;
                });
                auto nextId = vertexMap.size();

                // DEBUG
                for(auto &pair : vertexMap)
                {
                    std::cout << pair.first << "\t:\t" << pair.second << "\t(" << layerMap[pair.second] << ")" << std::endl;
                }

                // if an edge connects two vertices on layers that are more than 1 level apart, dummy vertices and edges need to be made
                for(auto &originalSource : graph.vertices())
                {
                    auto source = vertexMap[originalSource];
                    auto sourceLayer = layerMap[source];

                    for(auto &originalTarget : graph.outgoing(originalSource))
                    {
                        auto target = vertexMap[originalTarget];
                        auto targetLayer = layerMap[target];

                        // if two connected vertices are on layers more than 1 apart, this needs to be fixed by adding dummy vertices
                        if(std::abs(sourceLayer-targetLayer) > 1)
                        {
                            /* If sourceLayer and targetLayer differ, sourceLayer must be larger
                                since we only allow links from 'higher' layer to 'lower' layers
                             */
                            assert(sourceLayer > targetLayer);

                            std::vector<long> dummyPath;

                            for(int i=1; i<sourceLayer-targetLayer; i++)
                            {
                                layerMap[nextId] = sourceLayer - i;
                                graphWithDummies.insertEdge(i == 1 ? source : (nextId - 1), nextId);
                                dummyPath.push_back(i == 1 ? source : nextId - 1);
                                nextId++;
                            }
                            graphWithDummies.insertEdge(nextId-1, target);
                            dummyPath.push_back(target);
                            dummyPathMap[std::make_tuple(source, target)] = dummyPath;
                        }
                        // else we can simply add the edge to the dummy graph
                        else
                        {
                            graphWithDummies.insertEdge(source, target);
                        }

                    }
                }

                // return
                return make_tuple(vertexMap, layerMap, dummyPathMap);
            }

            /*! Shuffle all vertices within all layers, layer per layer,
                in order to minimize the total number of crossings.
                \param[in] layers
                \param[in] graphWithDummies
             */
            std::map<int, std::vector<long>> relativeLayout(const std::map<long, int> &layers, const IGraph<long> &graphWithDummies) const
            {
                // setup structures to hold return value
                std::map<int, std::vector<long>> shuffledLayers;

                // determine highest layer nr
                auto maxLayer = -1;
                for(auto &pair : layers)
                {
                    shuffledLayers[pair.second].push_back(pair.first);
                    maxLayer = std::max(maxLayer, pair.second);
                }
                assert(maxLayer >= 0);


                // layout each layer
                for(int i=maxLayer; i>=0; i--)
                {
                    if(i==maxLayer)
                    {
                        relativeLayoutById(i, shuffledLayers);
                    }
                    else
                    {
                        relativeLayoutByMetaHeuristic(i, shuffledLayers, graphWithDummies);
                    }
                }

                // return
                return shuffledLayers;
            }


            /*! Shuffle all vertices in the given layer, by sorting them based on their value.
                This typically only happens for the first layer of the layout.
                \param[in] layerNr  the nr of the layer being shuffled
             */
            void relativeLayoutById(int layerNr, std::map<int, std::vector<long>> &verticesByLayer) const
            {
                // asserts
                assert(layerNr >= 0);
                assert(verticesByLayer.find(layerNr) != verticesByLayer.end());

                auto &ids = verticesByLayer[layerNr];
                std::sort(ids.begin(), ids.end());
            }

            /*! Shuffle all vertices in the given layer, by running a metaheuristic on the order of the vertices.
                Permutations of the vertices are scored by the crossings they create.
                \param[in] layerNr  the nr of the layer being shuffled
             */
            void relativeLayoutByMetaHeuristic(int layerNr, std::map<int, std::vector<long>> &verticesByLayer, const IGraph<long> &graphWithDummies) const
            {
                /* asserts
                 */
                assert(layerNr >= 0);
                assert(verticesByLayer.find(layerNr) != verticesByLayer.end());

                /* nodes
                 */
                auto &vertices = verticesByLayer[layerNr];

                /* function to be optimized
                 */
                auto f = [layerNr, this, &verticesByLayer, &graphWithDummies](const std::vector<int> &input)
                {
                    return nofCrossingEdges(layerNr, verticesByLayer, graphWithDummies, input);
                };

                /* run the metaheuristic
                 */
                auto innerSearch = std::unique_ptr<meta::AbstractMetaHeuristic>(new meta::TabuSearch());
                auto search = meta::PermutationSearch(std::move(innerSearch));
                auto bestPermutation = search.findIntMin(f, vertices.size(), 1000);

                /* apply the result to the current layer
                 */
                auto temp = std::vector<long>();
                for(int i=0; i<bestPermutation.size(); i++)
                {
                    temp.push_back(vertices[bestPermutation[i]]);
                }

                assert(temp.size() == vertices.size());

                vertices = temp;
            }

            /*! \return the number of crossing edges between layer [layerNr] and layer [layerNr + 1] of a graph
                after having applied the given permutation to the vertices in layer [layerNr].
                \param[in] layerNr              the nr of the layer
                \param[in] verticesByLayer
                \param[in] graphWithDummies
                \param[in] permutation          the permutation being applied to the layer
             */
            int nofCrossingEdges(int layerNr, std::map<int, std::vector<long>> &verticesByLayer, const IGraph<long> &graphWithDummies, const std::vector<int> &permutation) const
            {
                assert(verticesByLayer.find(layerNr) != verticesByLayer.end());
                assert(verticesByLayer.find(layerNr+1) != verticesByLayer.end());
                assert(permutation.size() == verticesByLayer[layerNr].size());

                auto cross        = 0;
                auto &layer       = verticesByLayer[layerNr];
                auto &prevLayer   = verticesByLayer[layerNr+1];

                /* utility function to find the index of a given vertex when the permutation is applied
                 */
                auto findIndex = [&permutation, &layer](const long vertex)
                {
                    for(int i=0; i<layer.size(); i++)
                        if(layer[permutation[i]] == vertex)
                        {
                            return i;
                        }
                    return -1;
                };

                for(int i=1; i<prevLayer.size(); i++)
                {
                    auto source = prevLayer[i];
                    for(auto &target : graphWithDummies.outgoing(source))
                    {
                        auto targetPos = findIndex(target);
                        assert(targetPos >= 0 && targetPos < layer.size());

                        for(int j=0; j<i; j++)
                        {
                            for(auto &intersectTarget : graphWithDummies.outgoing(prevLayer[j]))
                            {
                                auto intersectTargetPos = findIndex(intersectTarget);
                                //std::cout << "source : " << source << ", target : " << target << ", targetPos : " << targetPos << ", intersectSource : " << prevLayer[j] << ", intersectTarget : " << intersectTarget << ", intersectTargetPos : " << intersectTargetPos << std::endl;
                                assert(intersectTargetPos >= 0 && intersectTargetPos < layer.size());
                                cross += (intersectTargetPos > targetPos) ? 1 : 0;
                            }
                        }
                    }
                }

                return cross;
            }

            /*!
             */
            std::map<long, std::tuple<int,int>> absoluteLayout(const std::map<int, std::vector<long>> &relativeLayout, const std::map<T,long> &vertexMapping, const typename graphUI::AbstractGraphLayout<T>::SizeFunctionType &sizeFunction)
            {
                // structures to hold return value
                std::map<long, std::tuple<int,int>> pos;

                // margins
                auto xMargin = graphUI::AbstractGraphLayout<T>::getMinXMargin();
                auto yMargin = graphUI::AbstractGraphLayout<T>::getMinYMargin();

                // reverse map
                std::map<long, T> reverseVertexMapping;
                for(auto &pair : vertexMapping)
                {
                    reverseVertexMapping[pair.second] = pair.first;
                }

                // modified size function to take into account dummy vertices
                auto modSizeFun = [&reverseVertexMapping, &sizeFunction, xMargin, yMargin](long id)
                {
                    if(reverseVertexMapping.find(id) == reverseVertexMapping.end())
                    {
                        return std::make_tuple(0, 0);
                    }
                    return sizeFunction(reverseVertexMapping[id]);
                };

                // assign each vertex a position
                auto xpos = xMargin;
                auto ypos = yMargin;
                for(auto &pair : relativeLayout)
                {
                    // reset xpos
                    xpos = xMargin;

                    // layout a layer
                    auto &layerLayout = pair.second;
                    auto maxHeight = 0;
                    for(int i=0; i<layerLayout.size(); i++)
                    {
                        auto w = std::get<0>(modSizeFun(layerLayout[i]));
                        auto h = std::get<1>(modSizeFun(layerLayout[i]));

                        // assign position
                        pos[layerLayout[i]] = std::make_tuple(xpos, ypos);

                        // keep track of max height of vertex in this layer
                        maxHeight = std::max(h, maxHeight);

                        // move to next xpos
                        xpos += w;
                        xpos += xMargin;
                    }

                    // move to next ypos
                    ypos += maxHeight;
                    ypos += yMargin;
                }

                // return
                return pos;
            }

    };
}

#endif // HIERARCHICALGRAPHLAYOUT_HPP  
