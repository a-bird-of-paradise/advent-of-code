#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "octopus.hpp"
#include <iomanip>

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    std::vector<std::vector<int>> flashed = Context.lines;

    for(auto & i : flashed) for(auto & j : i) j=0;

    std::vector<aoc::point_t> to_flash;
    unsigned long long flashes = 0;

    int step = 0, sum = 0;

    bool loop = true;

    while(loop)
    {
        // bump everyone

        for(std::size_t i = 0; i < Context.lines.size(); ++i) {
            for(std::size_t j = 0; j < Context.lines[i].size(); ++j) {
                Context.lines[i][j]++; 
                if(Context.lines[i][j] > 9 ) to_flash.emplace_back(std::make_pair(i,j));
            }
        }

        // flash everyone

        while(to_flash.size()>0)
        {
            std::size_t x,y;
            x = to_flash.back().first;
            y = to_flash.back().second;
            to_flash.pop_back();

            if(flashed[x][y] == 0)
            {
                flashed[x][y] = 1; flashes++;

                if(x>0 && y > 0) {
                    Context.lines[x-1][y-1]++;
                    if (Context.lines[x-1][y-1] > 9 && flashed[x-1][y-1] == 0)
                        to_flash.emplace_back(std::make_pair(x-1,y-1));
                }

                if(x>0 && true) {
                    Context.lines[x-1][y]++;
                    if (Context.lines[x-1][y] > 9 && flashed[x-1][y] == 0)
                        to_flash.emplace_back(std::make_pair(x-1,y));
                }

                if(x>0 && y < Context.lines.size() - 1 ) {
                    Context.lines[x-1][y+1]++;
                    if (Context.lines[x-1][y+1] > 9 && flashed[x-1][y+1] == 0)
                        to_flash.emplace_back(std::make_pair(x-1,y+1));
                }

                if(x < Context.lines.size() - 1  && y > 0) {
                    Context.lines[x+1][y-1]++;
                    if (Context.lines[x+1][y-1] > 9 && flashed[x+1][y-1] == 0)
                        to_flash.emplace_back(std::make_pair(x+1,y-1));
                }

                if(x < Context.lines.size() - 1  && true) {
                    Context.lines[x+1][y]++;
                    if (Context.lines[x+1][y] > 9 && flashed[x+1][y] == 0)
                        to_flash.emplace_back(std::make_pair(x+1,y));
                }

                if(x < Context.lines.size() - 1  && y < Context.lines.size() - 1 ) {
                    Context.lines[x+1][y+1]++;
                    if (Context.lines[x+1][y+1] > 9 && flashed[x+1][y+1] == 0)
                        to_flash.emplace_back(std::make_pair(x+1,y+1));
                }

                if(true && y > 0) {
                    Context.lines[x][y-1]++;
                    if (Context.lines[x][y-1] > 9 && flashed[x][y-1] == 0)
                        to_flash.emplace_back(std::make_pair(x,y-1));
                }

                if(true && y < Context.lines.size() - 1 ) {
                    Context.lines[x][y+1]++;
                    if (Context.lines[x][y+1] > 9 && flashed[x][y+1] == 0)
                        to_flash.emplace_back(std::make_pair(x,y+1));
                }
            }
        }

        sum = 0;
        for(auto & i : flashed) for(auto & j : i) j = 0;
        for(auto & i : Context.lines) for(auto & j : i) j = j > 9 ? 0 : j;
        for(auto & i : Context.lines) for(auto & j : i) sum += j;

        step++;

        if(step == 100) {
            std::cout << std::setw(4) << step << " " << flashes << std::endl;
        }

        if(sum == 0)
        {
            std::cout << std::setw(4) << step << std::endl;
            if(step > 100) loop = false;
        }

    }

    return 0;
}