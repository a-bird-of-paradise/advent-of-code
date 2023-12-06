#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <boost/icl/interval_set.hpp>

long long apply_map_single ( long long source, const std::vector<aoc::triple>& map);

std::vector<long long> 
    apply_map(
        const std::vector<long long>& source, 
        const std::vector<aoc::triple>& map
    );

boost::icl::interval_set<int64_t>
    apply_map_set(
        const boost::icl::interval_set<int64_t>& source,
        const std::vector<aoc::triple>& map
    );
