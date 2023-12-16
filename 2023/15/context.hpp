#pragma once
#include "location.hh"
#include <map>
#include <string_view>
#include <vector>

namespace aoc
{

    class lens
    {
    public:
        lens(std::string_view lab, uint16_t pow) : label(lab), power(pow) {}
        lens() : power(0) {}
        
        const bool operator < ( const lens &r ) const{
            return std::lexicographical_compare(
                label.begin(), 
                label.end(),
                r.label.begin(),
                r.label.end());
        }

        const bool operator == ( const lens& r) const {
            return label == r.label;
        }

        const bool operator == ( std::string_view r) const {
            return label == r;
        }

        std::string label;
        uint16_t power;
    };

    class context
    {
    public:
        context() : answer_1(0), answer_2(0) { loc.initialize(); boxes.resize(256);  }
        location loc;
        uint64_t answer_1, answer_2;
        std::vector<std::vector<lens>> boxes;
    };
}
