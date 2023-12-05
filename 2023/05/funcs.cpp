#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
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

long long apply_map_single ( long long source, const std::vector<aoc::triple>& map)
{
    long long answer = source;

    for(const auto& t : map)
    {
        if(t.source_range_start <= source && source < t.source_range_start + t.range_length)
        {
            answer = t.dest_range_start + source - t.source_range_start;
            break;
        }
    }

    return answer;
}