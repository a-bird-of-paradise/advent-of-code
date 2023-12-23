#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <ranges>
#include <set>

static int64_t max_seen = 0;

auto dfs(
    const std::map<int64_t, std::vector<int64_t>>& graph,
    const int64_t goal,
    const int64_t current,
    std::set<int64_t>& current_path,
    std::vector<std::set<int64_t>>& paths)
{
    // we're here
    current_path.insert(current);

    // at the end?
    if(current == goal) {
        paths.push_back(current_path);
        current_path.erase(current);
        if(current_path.size() > max_seen) {
            max_seen = current_path.size();
            std::cout << "!!! " << max_seen << '\n';
        }
        return;
    }

    // try each neighbour 

    for(const auto& n : graph.at(current))
    {
        if(!current_path.contains(n)) {
            dfs(graph,goal,n,current_path,paths);
        }
    }

    current_path.erase(current);
    return;
}


auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1;

    std::map<int64_t, std::vector<int64_t>> graph = build_graph(Context.lines);
    std::set<int64_t> current_path;
    std::vector<std::set<int64_t>> paths;

    int64_t start = get_index(1,0);
    int64_t end = get_index(Context.lines[0].size()-2, Context.lines.size()-1);

    dfs(graph,end,start,current_path,paths);

    std::vector<int64_t> sizes;
    for(const auto & p : paths) sizes.push_back(p.size()-1);

    std::cout << *std::max_element(std::begin(sizes),std::end(sizes)) << '\n';

    // part 2

    graph = build_graph_2(Context.lines);
    current_path.clear();
    paths.clear();
    sizes.clear();

    dfs(graph,end,start,current_path,paths);

    for(const auto & p : paths) sizes.push_back(p.size()-1);

    std::cout << *std::max_element(std::begin(sizes),std::end(sizes)) << '\n';

    return 0;
}