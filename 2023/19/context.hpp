#pragma once
#include "location.hh"
#include <map>
#include <vector>
#include <boost/icl/interval_set.hpp>

namespace aoc
{
    struct part{int64_t x,m,a,s;};

    struct rule { char type, compare, letter; bool is_accept; std::string name; int64_t num; };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<part> parts;
        std::map<std::string, std::vector<rule>> workflows;
    };

    struct rating_set
    {
        std::string workflow;
        int64_t rule_no;
        boost::icl::discrete_interval<int64_t> x,m,a,s;
    };
}
