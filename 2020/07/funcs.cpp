#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto num_children(const aoc::context& ctx, const aoc::bag_colour_t& colour) -> int32_t
{
    int32_t answer = 0;

    static std::map<aoc::bag_colour_t,int32_t> cache;

    auto at = cache.find(colour);
    if(at != cache.end()) return at->second;

    for(const auto& child : ctx.bags.at(colour))
    {
        answer += child.second; // one of each child bag
        answer += child.second * num_children(ctx, child.first); // and whatever is in it
    }

    cache[colour] = answer;

    return answer;
}