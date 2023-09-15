#pragma once
#include <vector>

namespace aoc
{

    typedef std::pair<std::size_t, std::size_t> point_t;

    class octopus
    {
    public:

    octopus(int energy_level, std::size_t x, std::size_t y)
        :
        energy_level(energy_level), location(std::make_pair(x,y)), flashed(false)
        {}
    
    int energy_level;
    point_t location;
    std::vector<std::size_t> neighbours;
    bool flashed;
    const int distance(const octopus& other);
    };
}