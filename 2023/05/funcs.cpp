#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

long long apply_map_single ( long long source, const std::vector<aoc::triple>& map)
{
    long long answer = source;
    for(const auto& t : map){
        if(t.source_range_start <= source && source < t.source_range_start + t.range_length){
            answer = t.dest_range_start + source - t.source_range_start;
            break;
        }
    }
    return answer;
}

std::vector<long long> 
    apply_map(
        const std::vector<long long>& source, 
        const std::vector<aoc::triple>& map
    )
{
    std::vector<long long> answer;
    for(const auto& s : source) answer.push_back(apply_map_single(s,map));
    return answer;
}


boost::icl::interval_set<int64_t>
    apply_map_set(
        const boost::icl::interval_set<int64_t>& source,
        const std::vector<aoc::triple>& map
    )
{
    boost::icl::interval_set<int64_t> unvisited = source, answer, intersection;

    for(const auto& t : map)
    {
        boost::icl::discrete_interval<int64_t> 
            mapped_range(t.source_range_start, t.source_range_start + t.range_length);

        intersection = unvisited & mapped_range;
        unvisited -= mapped_range;

        for(auto&& ival : intersection)
        {
            boost::icl::discrete_interval<int64_t> 
                mutated(ival.lower() + t.dest_range_start - t.source_range_start,
                    ival.upper() + t.dest_range_start - t.source_range_start);

            answer += mutated;
        }

    }

    answer += unvisited;

    return answer;
}