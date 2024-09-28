#pragma once
#include "location.hh"
#include <vector>
#include <map>

namespace aoc
{

    typedef std::vector<int> ints;
    typedef std::vector<aoc::ints> intss;

    enum class rule_type { terminal, nonterminal };

    struct rule { rule_type type; char c; aoc::intss rules; };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;

        std::map<int, aoc::rule> rules;

        std::vector<std::string> strings;
    };
}
