#pragma once
#include "location.hh"
#include <queue>
#include <deque>
#include <stack>

namespace aoc
{

    class packet
    {
    public:

        packet() : type_id(0), version(0), data_value(0) {}

        int type_id;
        int version;
        unsigned long long data_value;
        unsigned long long eval(void);

        std::vector<packet> children;
    };

    class context
    {
    public:
        context() 
            : 
            version_sum(0),
            bit_counter(0) 
            { 
                loc.initialize(); 
            }

        location loc;
        std::queue<int> tokens;
        unsigned long long bit_counter, version_sum;

        std::stack< std::pair<int, long long> > terminator_counter;

    };
}