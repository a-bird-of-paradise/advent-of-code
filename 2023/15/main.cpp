#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <map>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    std::vector<std::string> lines = Context.lines;
    while(tick_n(lines)!=0);
    std::cout << load(lines) << '\n';

    // part 2

    std::vector<uint32_t> cycle_scores;
    std::map<uint32_t,std::vector<uint32_t>> scores;
    for(auto i = 0; i < 172; ++i) {
        cycle_scores.push_back(load(Context.lines));
        scores[load(Context.lines)].push_back(i);
        cycle(Context.lines);

    }
    
    // test input: front = 4, length = 7 
    // so n large = [(n-4) % 7 ] + 4
    // main : 84, x 157 - 85 + 1

    std::cout << "Test answer: " << cycle_scores[((1000000000-4)  %  7) + 4 ] << '\n';
    std::cout << "Real answer: " << cycle_scores[((1000000000-100) % 72) + 100] << '\n';

    // too low99099, 99111
    // 99118?
    // too high 99165

    return 0;
}
