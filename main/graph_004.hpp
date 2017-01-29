#include <fstream>
#include <functional>
#include <iostream>

#include "graph/randomgraphgenerator.hpp"
#include "graph/stringgraph.hpp"
#include "xml/graphml.hpp"

using namespace graph;
using namespace XML;
using namespace std;

int main()
{
    // generate random graph
    auto graphPtr = graph::algorithm::generate(3, 5, 3, 8);

    // IGraph<std::string>
    auto stringFunction = [](long l)
    {
        return "node_" + std::to_string(l);
    };

    auto sGraphPtr = graph::algorithm::stringify<long>(*(graphPtr.get()), stringFunction);

    // persist
    ofstream ofs;
    ofs.open("/home/joris/random_graph.graphml");
    ofs << *(sGraphPtr.get());
    ofs.close();

    // return
    return 0;
}