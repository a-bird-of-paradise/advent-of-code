#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>
#include <algorithm>
#include <cmath>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    std::set<std::size_t> used_rows, used_columns, all_rows, all_columns;
    std::vector<std::size_t> empty_rows, empty_columns;

    std::vector<std::pair<int64_t,int64_t>> galaxies;

    for(std::size_t i = 0; i < Context.field.size(); ++i) {
        for(std::size_t j = 0; j < Context.field[i].size(); ++j) {
            if(Context.field[i][j] == '#') {
                used_rows.insert(i);
                used_columns.insert(j);
                galaxies.emplace_back(i,j);
            }
            if(i == 1) all_columns.insert(j);
        }
        all_rows.insert(i);
    }

    std::set_difference(
        std::begin(all_rows), 
        std::end(all_rows),
        std::begin(used_rows),
        std::end(used_rows),
        std::back_inserter(empty_rows)
    );

    std::set_difference(
        std::begin(all_columns), 
        std::end(all_columns),
        std::begin(used_columns),
        std::end(used_columns),
        std::back_inserter(empty_columns)
    );

    // part 1

    int64_t answer_1 = 0, answer_2 = 0; 

    for(auto i = galaxies.begin(); i != galaxies.end(); ++i) {
        for(auto j = std::next(i); j != galaxies.end(); ++j) {
            int64_t n_empty_rows, n_empty_columns;

            n_empty_rows = std::count_if(
                std::begin(empty_rows),
                std::end(empty_rows),
                [&](auto x){return x >= std::min(i->first,j->first) && x <= std::max(i->first,j->first);}
            );

            n_empty_columns = std::count_if(
                std::begin(empty_columns),
                std::end(empty_columns),
                [&](auto x){return x >= std::min(i->second,j->second) && x <= std::max(i->second,j->second);}
            );

            answer_1 += std::abs(i->second - j->second);
            answer_1 += std::abs(i->first - j->first);
            answer_1 += (2-1) * n_empty_rows;
            answer_1 += (2-1) * n_empty_columns;

            answer_2 += std::abs(i->second - j->second);
            answer_2 += std::abs(i->first - j->first);
            answer_2 += (1000000-1) * n_empty_rows;
            answer_2 += (1000000-1) * n_empty_columns;
        }
    }

    std::cout << answer_1 << '\n' << answer_2 << '\n';

    return 0;
}
