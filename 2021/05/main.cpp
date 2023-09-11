#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    const long max_dim = 1000;

    Parser.parse();

    long(*field)[max_dim] = new long[max_dim][max_dim]; // C++ doesn't let these be non-const...

    for(int i = 0; i < max_dim; i++){
        for(int j = 0; j < max_dim; j++) {
            field[i][j]=0;
        }
    }

    for( auto& i : Context.lines )
    {
        // horizontal or vertical
        if(i.first.first == i.second.first)
        {
            for(int j = std::min(i.first.second,i.second.second);
                    j <= std::max(i.first.second,i.second.second);
                    j++) {
                        field[j][i.first.first]++;
                    }
        }
        else if(i.first.second == i.second.second)
        {
            for(int j = std::min(i.first.first,i.second.first);
                    j <= std::max(i.first.first,i.second.first);
                    j++) {
                        field[i.first.second][j]++;
                    }
        }
    }

    long answer = 0;

    for(int i = 0; i < max_dim; i++){
        for(int j = 0; j < max_dim; j++) {
            if(field[i][j] > 1) {
                answer++;
            }
        }
    }

    std::cout << answer << std::endl;

    long dx, dy, n;
    aoc::point_t point;

    for( auto& i : Context.lines)
    {
        // horizontal or vertical
        if(i.first.first == i.second.first || i.first.second == i.second.second) continue;

        n = i.first.first - i.second.first;
        if(n<0) n *= -1;
        dx = i.second.first - i.first.first;
        dy = i.second.second - i.first.second;
        dx /= n;
        dy /= n;

        point.first = i.first.first;
        point.second = i.first.second;

        for(int j = 0; j <= n; j++)
        {
            field[point.second][point.first]++;
            point.first += dx;
            point.second += dy;
        }
    }

    answer = 0;

    for(int i = 0; i < max_dim; i++){
        for(int j = 0; j < max_dim; j++) {
            if(field[i][j] > 1) {
                answer++;
            }
        }
    }

    std::cout << answer << std::endl;

    delete [] field;

    return 0;
}