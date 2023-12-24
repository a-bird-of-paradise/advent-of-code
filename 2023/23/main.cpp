#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <ranges>
#include <set>


auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1;

    std::map<int64_t, std::vector<int64_t>> graph = build_graph(Context.lines);
    std::vector<int64_t> current_path;
    std::vector<std::vector<int64_t>> paths;

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
    
    std::map<int64_t, std::vector<std::pair<int64_t,int64_t>>> reduced = compact(graph,start,end);

    std::map<int64_t, std::vector<int64_t>> reduced_adjacency;
    std::map<int64_t, std::map<int64_t,int64_t>> lengths;

    for(const auto & g : reduced) {
        for(const auto & p : g.second) {
            reduced_adjacency[g.first].push_back(p.first);
            lengths[g.first][p.first]=p.second;
        }
    }

    dfs(reduced_adjacency,end,start,current_path,paths);

    int64_t max_length = INT64_MIN;

    for(const auto & path : paths) {
        int64_t length = 0, current = path.front();

        for(int i = 1; i < path.size(); ++i) {
            length += lengths[current][path[i]];
            current = path[i];
        }
        max_length = std::max(max_length,length);
    }

    std::cout << max_length << '\n';

    return 0;
}