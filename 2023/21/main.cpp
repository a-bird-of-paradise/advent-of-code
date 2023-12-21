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

    std::vector<std::string> current = Context.lines, next;

    int64_t answer_1 = 0;
    
    for(auto&& r : Context.lines) for(auto&& c : r) if (c == 'S') c = '.';

    std::map<std::vector<std::string>,std::vector<int>> states;

    for(int i = 0; i < 130; ++i) 
    {
        next = Context.lines;

        if(i == 0) 
        {
            for(int y = 0; y < current.size(); ++y)
            {
                for(int x = 0; x < current[y].size(); ++x)
                {
                    if(current[y][x] == 'S')
                    {
                        next[y-1][x] = next[y-1][x] == '.' ? 'O' : '#';
                        next[y+1][x] = next[y+1][x] == '.' ? 'O' : '#';
                        next[y][x-1] = next[y][x-1] == '.' ? 'O' : '#';
                        next[y][x+1] = next[y][x+1] == '.' ? 'O' : '#';
                    }
                }
            }
        }
        else {
            for(int y = 0; y < current.size(); ++y)
            {
                for(int x = 0; x < current[y].size(); ++x)
                {
                    if(current[y][x] == '#') continue;
                    if(y > 0)
                        if(current[y-1][x] == 'O')
                            next[y][x] = 'O';
                    if(x > 0)
                        if(current[y][x-1] == 'O')
                            next[y][x] = 'O';
                    if(y < current.size() - 1)
                        if(current[y+1][x] == 'O')
                            next[y][x] = 'O';
                    if(x < current[y].size() - 1)
                        if(current[y][x+1] == 'O')
                            next[y][x] = 'O';
                }
            }
        }

        current = next;

        if(i == 63) 
            for(const auto & l : current) 
                for(const auto & x : l) 
                    if(x == 'O') 
                        answer_1++;

        states[current].push_back(i);
    }

    std::cout << answer_1 << '\n';

    for(const auto & p : states)
    {
        for(const auto & l : p.first) std::cout << l << '\n';
        for(const auto & v : p.second) std::cout << v << ' ';
        std::cout << '\n';
    }
    return 0;
}