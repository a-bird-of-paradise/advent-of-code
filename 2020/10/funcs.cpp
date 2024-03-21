#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <algorithm>

auto built_in_rating(const std::vector<int64_t>& adapters) -> int64_t
{
    return (*std::ranges::max_element(adapters))+3;
}

auto count_paths(
    const std::map<int64_t, std::vector<int64_t>>& graph,
    const int64_t from,
    const int64_t last
) -> int64_t
{
    // need a cache

    static std::map<int64_t,int64_t> cache;

    // check cache
    auto cache_hit = cache.find(from);
    if(cache_hit != cache.end()) return cache_hit->second;

    // terminal node
    if(from == last) return 1;

    // recurse through

    for(const auto& next : graph.at(from))
        cache[from] += count_paths(graph,next,last);

    return cache[from];
}