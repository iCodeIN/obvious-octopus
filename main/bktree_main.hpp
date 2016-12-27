#include <iostream>
#include <string>

#include "graph/bktree.hpp"

using namespace graph;

int main()
{
    auto metric = [](const std::string& s0, const std::string& s1)
    {
        return 0;
    };

    BKTree<std::string> t(metric);

    // return
    return 0;
}