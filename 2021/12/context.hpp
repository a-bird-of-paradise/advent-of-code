#pragma once
#include "location.hh"
#include <vector>
#include <map>
#include <set>

namespace aoc
{

    typedef std::map<std::string,std::vector<std::string>> graph_t;

    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::map<std::string,std::vector<std::string>> graph;
        std::set<std::string> small_nodes, large_nodes;
    };
}