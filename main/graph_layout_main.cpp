#include "graph/igraph.hpp"
#include "graph/adjecencylistgraph.hpp"

#include "graph_ui/hierarchicalgraphlayout.hpp"
#include "graph_ui/snaptogridgraphlayout.hpp"
#include "graph_ui/defaultgraphrenderer.hpp"

#include "svg/svg.hpp"

#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <tuple>

using namespace std;
using namespace graph;
using namespace graphUI;
using namespace SVG;

int main()
{
    // setup graph
    std::set<std::string> nodes;
    nodes.insert("F");
    nodes.insert("G");
    nodes.insert("H");
    nodes.insert("A");
    nodes.insert("B");
    nodes.insert("C");
    nodes.insert("D");
    nodes.insert("E");
    nodes.insert("I");

    auto f = [](const std::string &source, const std::string &target)
    {
        return   (source == "A" && target == "C") ||
                 (source == "A" && target == "D") ||

                 (source == "B" && target == "C") ||
                 (source == "B" && target == "E") ||

                 (source == "C" && target == "F") ||
                 (source == "C" && target == "G") ||

                 (source == "D" && target == "F") ||
                 (source == "D" && target == "G") ||

                 (source == "E" && target == "G") ||

                 (source == "F" && target == "H") ||

                 (source == "G" && target == "H") ||

                 (source == "A" && target == "I");
    };

    auto graph = unique_ptr<IGraph<string>>(new AdjecencyListGraph<string>(nodes, f));

    // set preferences for pixel size of vertices
    auto vertexSize = 30;
    auto boundingBox = [vertexSize](const string &vertexId)
    {
        return make_tuple(vertexSize, vertexSize);
    };

    // setup layout parameters
    auto margin = 50;
    auto layoutMgr0 = unique_ptr<HierarchicalGraphLayout<string>>(new HierarchicalGraphLayout<string>());
    layoutMgr0.get()->setMaxXMargin(margin);
    layoutMgr0.get()->setMinXMargin(margin);
    layoutMgr0.get()->setMaxYMargin(margin);
    layoutMgr0.get()->setMinYMargin(margin);

    // apply 'snap to grid' layout adaptor
    auto layoutMgr1 = unique_ptr<SnapToGridGraphLayout<string>>(new SnapToGridGraphLayout<string>(std::move(layoutMgr0)));
    auto layout = layoutMgr1->layout(*graph.get(), boundingBox);

    // build svg
    auto renderer = unique_ptr<DefaultGraphRenderer<string>>(new DefaultGraphRenderer<string>());
    auto img = renderer.get()->render(*(graph.get()), layout, boundingBox);

    // flip
    img.get()->flipHorizontal();

    // persist svg
    ofstream myfile;
    myfile.open ("demo_graph_output.svg");
    myfile << *(img.get()) << std::endl;
    myfile.close();

    // return
    return 0;
}
