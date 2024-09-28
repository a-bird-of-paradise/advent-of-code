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

    aoc::field_t answer = backtrack_root(Context.tiles);

    backtrack(Context.tiles,answer);

    uint64_t part_1;
    std::size_t n = answer.size();

    part_1 = 1;
    part_1 *= answer[0][0].t ;
    part_1 *= answer[0][n-1].t ;
    part_1 *= answer[n-1][0].t ;
    part_1 *= answer[n-1][n-1].t ;
    
    std::cout << part_1 << '\n';

    aoc::tile rendered = render(Context.tiles,answer);
    std::set<std::pair<int,int>> used_points;

    aoc::tile monster = get_monster();

    for(std::size_t row = 0; row < rendered.size(); ++row) 
        for(std::size_t col = 0; col < rendered[row].size(); ++col) 
            for(int k = 1; k <= 8; ++k) 
                if(monster_could_be_at(apply_rotation(k,rendered),row,col)) 
                    for(std::size_t l = 0; l < monster.size(); ++l) 
                        for(std::size_t m = 0; m < monster[l].size(); ++m) 
                            if(monster[l][m] == '#') 
                                used_points.insert(std::make_pair(l + row, m + col));

    int n_hash = 0;
    for(const auto & row : rendered) 
        for(const auto & col : row) 
            if(col == '#') 
                n_hash++;

    std::cout << n_hash - used_points.size() << std::endl;

    return 0;
}