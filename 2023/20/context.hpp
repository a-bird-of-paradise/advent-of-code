#pragma once
#include "location.hh"
#include <map>
#include <vector>

namespace aoc
{

    enum class node_type { BROADCASTER, FLIPFLOP, CONJUNCTION, BUTTON, SPECIAL };

    struct module
    {
        node_type type;
        std::vector<std::string> destinations;
        bool flip_flop_state, output_high;
        std::map<std::string,bool> conjunction_memory;

    };

    struct pulse
    {
        std::string from, to;
        bool is_high;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::map<std::string,module> modules;
    };
}
