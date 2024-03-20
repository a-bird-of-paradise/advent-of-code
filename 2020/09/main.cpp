#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    // part 1a -  5 preamble

    std::set<int64_t> priors;
    int64_t part2_target;

    if(Context.ints.size() > 5)
    {
        std::size_t current_int = 5, i, j;

        while(current_int < Context.ints.size()) {
            priors.clear();
            for(i = current_int - 5; i < current_int; ++i) {
                for(j = i+1; j < current_int; ++j) {
                    if(Context.ints[i] == Context.ints[j]) continue;
                    priors.insert(Context.ints[i] + Context.ints[j]);
                }
            }
            if(!priors.contains(Context.ints[current_int])) {
                std::cout << Context.ints[current_int] << '\n';
                part2_target = Context.ints[current_int];
                break;
            }
            current_int++;
        }
    }

    // part 1b - 25 preamble

    if(Context.ints.size() > 25)
    {
        std::size_t current_int = 25, i, j;

        while(current_int < Context.ints.size()) {
            priors.clear();
            for(i = current_int - 25; i < current_int; ++i) {
                for(j = i+1; j < current_int; ++j) {
                    if(Context.ints[i] == Context.ints[j]) continue;
                    priors.insert(Context.ints[i] + Context.ints[j]);
                }
            }
            if(!priors.contains(Context.ints[current_int])) {
                std::cout << Context.ints[current_int] << '\n';
                part2_target = Context.ints[current_int];
                break;
            }
            current_int++;
        }
    }

    // part 2 - runs

    bool done = false;

    for(std::size_t i = 0; i < Context.ints.size(); ++i) {
        int64_t accum = 0, min_seen = INT64_MAX, max_seen = INT64_MIN;
        for(std::size_t j = i; j < Context.ints.size(); ++j) {
            accum += Context.ints[j];
            min_seen = Context.ints[j] < min_seen ? Context.ints[j] : min_seen;
            max_seen = Context.ints[j] > max_seen ? Context.ints[j] : max_seen;
            if(accum == part2_target) {
                std::cout << min_seen + max_seen << '\n';
                done = true;
                break;
            }
        }
        if(done) break;
    }
    return 0;
}
