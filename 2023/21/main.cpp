#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include "state_dumper.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    //dump_states(Context.lines);

    if(Context.lines.size() != 131) return 0;  
    
    std::map<std::vector<std::string>,std::vector<int>> states_central
        = state_maker(Context.lines,263);

    std::map<std::pair<int64_t,int64_t>, std::vector<std::pair<int64_t,int64_t>>> table;

    for(int64_t y = 0; y < Context.lines.size(); ++y) {
        for(int64_t x = 0; x < Context.lines[y].size(); ++x) {
            table[{y,x}] = neighbours(Context.lines,{y,x});
        }
    }

    std::cout << reached(table,64,{65,65}).size() << '\n';

    std::set<std::pair<int64_t,int64_t>> even_points, odd_points;

    even_points = reached(table,132,{65,65});
    odd_points = reached(table,133,{65,65});

    // body tiles done. Now for the apex tiles 

    std::set<std::pair<int64_t,int64_t>> top, left, right, bottom;
    top = reached(table,130,{0,65});
    left = reached(table,130,{65,0});
    right = reached(table,130,{65,130});
    bottom = reached(table,130,{130,65});

    // now the quarter corners . lost a day trying 65 and 66 not 64 uuuuuuuuuasd afasdf asjbfjkdnfjk najklf

    std::set<std::pair<int64_t,int64_t>>  top_left, top_right, bottom_left, bottom_right;

    top_left = reached(table,64,{0,0});
    top_right = reached(table,64,{0,130});
    bottom_left = reached(table,64,{130,0});
    bottom_right = reached(table,64,{130,130});

    // finally the three quarter corners 

    std::set<std::pair<int64_t,int64_t>>  full_top_left, full_top_right, full_bottom_left, full_bottom_right;

    full_top_left = reached(table,64+131,{0,0});
    full_top_right = reached(table,64+131,{0,130});
    full_bottom_left = reached(table,64+131,{130,0});
    full_bottom_right = reached(table,64+131,{130,130});

    constexpr int64_t N = 202300; // 202300 * 131 + 65 = desired steps

    std::cout 
        <<  N * N * even_points.size()                                      // even full squares
            +   (N-1)*(N-1)*odd_points.size()                               // odd full squraes
            +   top.size() + bottom.size() + left.size() + right.size()     //  apex points
            +   (N-1) * full_bottom_left.size()                             // 'interior' edges
            +   (N-1) * full_top_left.size() 
            +   (N-1) * full_top_right.size() 
            +   (N-1) * full_bottom_right.size()
            +   (N) * bottom_left.size()                                    // 'exterior' edges
            +   (N) * top_left.size() 
            +   (N) * top_right.size() 
            +   (N) * bottom_right.size()
        << std::endl;


    return 0;
}