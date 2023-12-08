#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <numeric>
#include <algorithm>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    std::string current = "AAA";
    std::string::const_iterator lr_it = Context.lr.begin();
    uint64_t counter = 0;

    while(current != "ZZZ"){
        counter++;

        if(*lr_it == 'L') {
            current = Context.nodes[current].first;
        } else if (*lr_it == 'R') {
            current = Context.nodes[current].second;
        }

        lr_it++;
        if(lr_it == Context.lr.end()) lr_it = Context.lr.begin();
    }

    std::cout << counter << '\n';

    // part 2

    std::vector<std::string> current_nodes;

    for(const auto& n : Context.nodes)
        if(n.first[2]=='A')
            current_nodes.push_back(n.first);

    std::vector<uint64_t> cycle_lengths;
    for(const auto & n : current_nodes) cycle_lengths.push_back(0);
    
    lr_it = Context.lr.begin();
    counter = 0;

    bool done = false;

    while(!done) {
        counter++;

        if(*lr_it == 'L') {
            for(auto&& n : current_nodes) {
                n = Context.nodes[n].first;
            }
        } else if (*lr_it == 'R') {
            for(auto&& n : current_nodes) {
                n = Context.nodes[n].second;
            }
        }

        done = true; 

        for(std::size_t i = 0; i < current_nodes.size(); ++i) {
            if(current_nodes[i][2] == 'Z' && cycle_lengths[i] == 0)
                cycle_lengths[i] = counter;
            
            if(cycle_lengths[i] == 0) done = false;
        }

        lr_it++;
        if(lr_it == Context.lr.end()) lr_it = Context.lr.begin();
    }

    uint64_t answer = 1;
    for(const auto& c: cycle_lengths) answer = std::lcm(answer,c);

    std::cout << answer << '\n';

    return 0;
}
