#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    class triple
    {
    public:
        triple() : dest_range_start(0), source_range_start(0), range_length(0) {}
        triple(long long dest, long long source, long long length) 
            :   dest_range_start(dest), source_range_start(source), range_length(length) {}
        auto operator<=>(const triple&) const = default;
        long long dest_range_start, source_range_start, range_length;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<long long> seeds;
        std::vector<triple> seed_to_soil, 
            soil_to_fertiliser,
            fertiliser_to_water,
            water_to_light,
            light_to_temperature,
            temperature_to_humidity,
            humidity_to_location;
    };

    typedef std::pair<long long, long long> segment;
}
