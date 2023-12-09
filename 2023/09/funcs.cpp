#include "funcs.hpp"
#include "scanner.hpp"
#include <algorithm>
#include "context.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto complete_history(const std::vector<int64_t>& history) -> std::pair<int64_t,int64_t>
{
    std::vector<std::vector<int64_t>> histories;

    histories.push_back(history);
    std::size_t level = 0;

    while(true)
    {
        std::vector<int64_t> diffs;
        int64_t max_seen = INT64_MIN, min_seen = INT64_MAX, diff;
        for(std::size_t i = 1; i < histories[level].size(); ++i)
        {
            diff = histories[level][i] - histories[level][i-1];
            max_seen = diff > max_seen ? diff : max_seen;
            min_seen = diff < min_seen ? diff : min_seen;
            diffs.push_back(diff);
        }
        histories.push_back(diffs);
        diffs.clear();
        level++;
        if(max_seen == min_seen && max_seen == 0LL) break;
    }

    int64_t part_1 = 0;
    for(const auto & n : histories) {
        part_1 += n.back();
    }

    for(std::size_t i = histories.size()-2; i < histories.size() ; i--)
    {
        int64_t current_front, lower_front;
        current_front = histories[i].front();
        lower_front= histories[i+1].front();
        histories[i].insert(std::begin(histories[i]),current_front - lower_front);
    }

    return std::make_pair(part_1,histories[0].front());
}

