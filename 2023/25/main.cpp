#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>
#include <queue>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    // adj matrix...

    std::vector<std::string> names;
    for(const auto & g : Context.graph) names.push_back(g.first);

    std::vector<std::vector<int>> adj_matrix; 
    adj_matrix.resize(names.size());
    for(auto&& v : adj_matrix) v.resize(names.size());

    for(const auto & g : Context.graph) {
        std::size_t r = std::distance(names.begin(),std::ranges::find(names,g.first));
        for(const auto & v : g.second) {
            std::size_t c = std::distance(names.begin(),std::ranges::find(names,v));
            adj_matrix[r][c] = 1;
        }
    }

    auto ans = globalMinCut(adj_matrix);

    std::cout << ans.second.size() *  (Context.graph.size() - ans.second.size()) << '\n';

    return 0;

    // 1287495 too high
}