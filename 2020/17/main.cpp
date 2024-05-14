#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>
#include <map>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    // part 1

    std::set<aoc::point> current_alive, next_alive;
    std::map<aoc::point,uint8_t> alive_neighbours;

    for(int16_t i = 0; i < Context.lines.size(); ++i)
        for(int16_t j = 0; j < Context.lines[i].size(); ++j)
            if(Context.lines[i][j] == '#')
                current_alive.insert({i,j,0});

    for(int16_t counter = 0; counter < 6; ++counter)
    {
        alive_neighbours.clear();
        next_alive.clear();

        for(const auto& p : current_alive)
        {
            for(auto dx = -1; dx <= 1; ++dx) {
                for(auto dy = -1; dy <= 1; ++dy) {
                    for(auto dz = -1; dz <= 1; ++ dz) {
                        if(dx == dy && dy == dz && dz == 0) continue;
                        alive_neighbours[{p.x+dx, p.y+dy, p.z+dz}]++;
                    }
                }
            }
        }

        for(const auto& p : current_alive)
        {
            if(alive_neighbours[p] == 2 || alive_neighbours[p] == 3) {
                next_alive.insert(p);
            }
        }

        for(const auto& p : alive_neighbours)
        {
            if(p.second != 3) continue;
            if(current_alive.contains(p.first)) continue;
            next_alive.insert(p.first);
        }

        current_alive = std::move(next_alive);

    }

    std::cout << current_alive.size() << '\n';

    // part 2

    std::set<aoc::point4> current_alive_2, next_alive_2;
    std::map<aoc::point4,uint8_t> alive_neighbours_2;

    for(int16_t i = 0; i < Context.lines.size(); ++i)
        for(int16_t j = 0; j < Context.lines[i].size(); ++j)
            if(Context.lines[i][j] == '#')
                current_alive_2.insert({0,i,j,0});

    for(int16_t counter = 0; counter < 6; ++counter)
    {
        alive_neighbours_2.clear();
        next_alive_2.clear();

        for(const auto& p : current_alive_2)
        {
            for(auto dw = -1; dw <= 1; ++dw) {
                for(auto dx = -1; dx <= 1; ++dx) {
                    for(auto dy = -1; dy <= 1; ++dy) {
                        for(auto dz = -1; dz <= 1; ++ dz) {
                            if(dw == dx && dx == dy && dy == dz && dz == 0) continue;
                            alive_neighbours_2[{p.w+dw,p.x+dx, p.y+dy, p.z+dz}]++;
                        }
                    }
                }
            }
        }

        for(const auto& p : current_alive_2)
        {
            if(alive_neighbours_2[p] == 2 || alive_neighbours_2[p] == 3) {
                next_alive_2.insert(p);
            }
        }

        for(const auto& p : alive_neighbours_2)
        {
            if(p.second != 3) continue;
            if(current_alive_2.contains(p.first)) continue;
            next_alive_2.insert(p.first);
        }

        current_alive_2 = std::move(next_alive_2);

    }

    std::cout << current_alive_2.size() << '\n';

    return 0;
}
