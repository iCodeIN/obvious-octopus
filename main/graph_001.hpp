#include "graph/adjecencylistgraph.hpp"

#include <string>
#include <memory>

using namespace graph;
using namespace std;

int main()
{
    auto graphPtr = std::unique_ptr<IGraph<string>>(new AdjecencyListGraph<string>());
    graphPtr->insertVertex("banaan");
    graphPtr->insertVertex("appel");
    graphPtr->insertEdge("banaan", "appel");

}