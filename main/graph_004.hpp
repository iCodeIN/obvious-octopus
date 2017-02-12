#include "graph/bfs.hpp"
#include "graph/randomgraphgenerator.hpp"
#include "graph/stringgraph.hpp"
#include "graph/eigenvalues.hpp"
#include "xml/graphml.hpp"

#include <fstream>
#include <functional>
#include <iostream>

using namespace graph;
using namespace XML;
using namespace std;

int main()
{
    // generate random graph
    auto graphPtr = graph::algorithm::generate( 2,  // min depth
                    4,  // max depth
                    4,  // min nof branches
                    16  // max nof branches
                                              );

    // calculate eigenvalues
    auto ev = graph::algorithm::eigenvalues(*graphPtr);
    for(auto &entry : ev)
    {
        cout << entry.first << "\t" << entry.second << endl;
    }

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