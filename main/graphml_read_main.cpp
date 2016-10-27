#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "xml/boostelementimpl.hpp"
#include "graph/adjecencylistgraph.hpp"

#include "graph_ui/hierarchicalgraphlayout.hpp"
#include "graph_ui/snaptogridgraphlayout.hpp"
#include "graph_ui/defaultgraphrenderer.hpp"

#include "svg/svg.hpp"

#include "xml/graphml.hpp"

#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <tuple>

using namespace std;
using namespace XML;
using namespace graph;
using namespace graphUI;

int main()
{
    ifstream ifs;
    ifs.open ("/home/joris/g0.xml", ifstream::in);

    // read graph
    auto graphPtr = (graph::IGraph<string>*) new AdjecencyListGraph<string>();
    ifs >> graphPtr;

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
    auto layout = layoutMgr1->layout(*graphPtr, boundingBox);

    // build svg
    auto renderer = unique_ptr<DefaultGraphRenderer<string>>(new DefaultGraphRenderer<string>());
    auto img = renderer.get()->render(*graphPtr, layout, boundingBox);

    // flip
    img.get()->flipHorizontal();

    // persist svg
    ofstream myfile;
    myfile.open ("demo_graph_output.svg");
    auto imgPtr = img.get();
    myfile << *imgPtr << std::endl;
    myfile.close();

    return 0;
}
