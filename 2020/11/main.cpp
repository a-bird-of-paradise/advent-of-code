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

    uint32_t counter;

    // part 1

    std::map<std::size_t, std::vector<std::size_t>> neighbours = build_neighbours(Context.lines);

    std::size_t max_index = neighbours.rbegin()->first;
    std::string prev_state(max_index+1,'.'), current_state(max_index+1,'.');
    std::size_t n_y = Context.lines[0].size();

    for(std::size_t i = 0; i < Context.lines.size(); ++i)
        for(std::size_t j = 0; j < Context.lines[i].size(); ++j) 
            prev_state[get_index(i,j,n_y)] = Context.lines[i][j];

    current_state = tick_state(prev_state,neighbours,n_y);

    while(current_state != prev_state) {
        prev_state = current_state;
        current_state = tick_state(prev_state,neighbours,n_y);
    }

    counter = 0;

    for(const auto& l : current_state) 
        if(l == '#') 
            counter++;

    std::cout << counter << '\n';   

    // part 2

    neighbours = build_neighbours_2(Context.lines);
    max_index = neighbours.rbegin()->first;
    for(auto&& c : prev_state) c = '.';
    for(auto&& c : current_state) c = '.';
    n_y = Context.lines[0].size();

    for(std::size_t i = 0; i < Context.lines.size(); ++i)
        for(std::size_t j = 0; j < Context.lines[i].size(); ++j) 
            prev_state[get_index(i,j,n_y)] = Context.lines[i][j];

    current_state = tick_state(prev_state,neighbours,n_y,5);

    while(current_state != prev_state) {
        prev_state = current_state;
        current_state = tick_state(prev_state,neighbours,n_y,5);
    }

    counter = 0;

    for(const auto& l : current_state) 
        if(l == '#') 
            counter++;

    std::cout << counter << '\n';   

    return 0;
}
