#pragma once
#include "location.hh"
#include <vector>
#include <tuple>

namespace aoc
{

    struct node {
        int x;
        int y;
        int id;
    };

    struct edge {
        int source_id;
        int destination_id;
        int weight;
    };

    struct graph {
        std::vector<node> nodes;
        std::vector<edge> edges;
    };

    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::vector<std::vector<int>> board;
    };
}