#pragma once
#include "location.hh"
#include <vector>
#include <array>
#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>

namespace aoc
{

    typedef std::array<int64_t,6> line_t;
    typedef boost::multiprecision::int128_t bigint_t;
    typedef boost::rational<bigint_t> rational_t;
    typedef std::pair<rational_t,rational_t> point_t;

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<line_t> lines;
    };
}
