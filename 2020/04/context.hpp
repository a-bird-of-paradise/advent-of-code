#pragma once
#include "location.hh"
#include <vector>
#include <map>

namespace aoc
{

    // type indicator
    enum class data_type {integer, hex, string, cm, in, four_int, nine_int};

    // union
    struct value_type { 
        data_type t;
        //union {
            int64_t int_val;
            int64_t four_int_val;
            int64_t nine_int_val;
            int64_t cm_val;
            int64_t in_val;
            std::string string_val;
            std::string hex_val;
        //};
    };

    typedef std::map<std::string,value_type> passport_t;

    /// Shared state object between Flex scanner, Bison parser and `main()`.
    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::string> field;
        std::vector<passport_t> passports;
    };
}
