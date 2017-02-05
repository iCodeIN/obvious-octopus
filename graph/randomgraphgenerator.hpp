#ifndef RANDOMGRAPHGENERATOR_HPP
#define RANDOMGRAPHGENERATOR_HPP

#include "graph/adjecencylistgraph.hpp"

#include <map>
#include <memory>
#include <random>
#include <stack>

namespace graph
{
    namespace algorithm
    {
        /*! Generates a random graph:
            1. build initial node, call it X
            2. create random number of nodes (between minNofBranch and maxNofBranches) to be connected to node X
            3. FOR EACH node N that was created
            4.  IF the depth of N is greater than the maxDepth OR greater than a random number generated between minDepth and maxDepth
            5.      DO NOTHING
            6.  ELSE
            7.      start again at step 2, using N as X
            \return randomly generated graph
         */
        static std::unique_ptr<IGraph<long>> generate(int minDepth, int maxDepth, int minNofBranches, int maxNofBrances)
        {
            auto retval = std::unique_ptr<IGraph<long>>(new AdjecencyListGraph<long>());

            std::random_device rd;
            std::mt19937 generator(rd());

            auto depthDistribution = std::uniform_int_distribution<int>(minDepth, maxDepth);
            auto branchDistribution = std::uniform_int_distribution<int>(minNofBranches, maxNofBrances);

            auto nextVertexId = 0L;

            std::map<long, int> depths;
            depths[nextVertexId] = 0;

            std::stack<long> toProcess;
            toProcess.push(nextVertexId);

            retval->insertVertex(nextVertexId);
            nextVertexId++;

            while(!toProcess.empty())
            {
                auto id = toProcess.top();
                auto d = depths[id];
                toProcess.pop();

                if(d >= maxDepth || d >= depthDistribution(generator))
                {
                    continue;
                }

                for(int i = 0; i < branchDistribution(generator); i++)
                {
                    // update graph
                    retval->insertVertex(nextVertexId);
                    retval->insertEdge(id, nextVertexId);

                    // update internal data structures
                    depths[nextVertexId] = d + 1;
                    toProcess.push(nextVertexId);

                    // get next id
                    nextVertexId++;
                }
            }

            // return
            return std::move(retval);
        }
    }
}

#endif // RANDOMGRAPHGENERATOR_HPP