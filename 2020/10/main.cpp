#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    // add the last thing

    Context.ints.push_back(built_in_rating(Context.ints));

    // destructive sort

    std::ranges::sort(Context.ints);

    // part 1

    int64_t open = 0; // start condition
    int64_t jolt1 = 0;
    int64_t jolt3 = 0;

    for(const auto& i : Context.ints)
    {
        if(open + 1 == i)       jolt1++;
        else if(open + 3 == i)  jolt3++;
        open = i;
    }

    std::cout << jolt1 * jolt3 << '\n';

    // part 2 
    // dfs into a graph to find paths...
    // make a graph

    std::map<int64_t, std::vector<int64_t>> graph;

    Context.ints.insert(std::begin(Context.ints),0);

    for(const auto& i : Context.ints)
        for(int64_t j = 1; j <= 3; ++j)
            if(std::ranges::binary_search(Context.ints, i+j))
                graph[i].push_back(i+j);

    std::cout << count_paths(graph,0,Context.ints.back()) << '\n';

    return 0;
}
