#include "graph/bktree.hpp"
#include "util/levenshtein.hpp"

#include <iostream>
#include <string>

using namespace graph;

int main()
{
    auto metric = [](const std::string& s0, const std::string& s1)
    {
        return util::Levenshtein::distance(s0, s1);
    };

    BKTree<std::string> t(metric);
    t.insert("aap");
    t.insert("aan");
    t.insert("haak");
    t.insert("haan");
    t.insert("pan");

    auto ws = t.find("aai", 1);
    for(auto &w : ws)
    {
        std::cout << w << std::endl;
    }

    // return
    return 0;
}