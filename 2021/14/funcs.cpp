#include "funcs.hpp"
#include <ranges>

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

std::string apply_rules(const std::string& base, const std::vector<aoc::rule_t>& rules)
{
    std::string answer;

    answer.push_back(base[0]);

    for(std::size_t i = 1; i < base.size(); i++)
    {
        if(auto result = std::ranges::find_if(rules.begin(), rules.end(),
            [&](const auto & r){ return std::get<0>(r) == base[i-1] && std::get<1>(r) == base[i];}); 
            result != rules.end())
                answer.push_back(std::get<2>(*result));
                
        answer.push_back(base[i]);
    }

    return answer;
}

long long score_1(const std::string& answer)
{
    std::map<char,long long> histogram;
    for(auto i : answer) histogram[i]++;
    long long max = INT64_MIN, min = INT64_MAX;
    for(auto i : histogram) {
        max = i.second > max ? i.second : max;
        min = i.second < min ? i.second : min;
     //    std::cout << i.first << " " << i.second << std::endl;
    }
    return max-min;
}