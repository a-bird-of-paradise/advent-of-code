#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <map>
#include <numeric>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    std::map<aoc::number,std::vector<aoc::symbol>> adjacencies;
    std::map<aoc::symbol,std::vector<aoc::number>> adjacencies_2;
    std::map<int, std::vector<aoc::symbol>> row_to_sym_map;

    for(const auto & syms: Context.symbols) {
        row_to_sym_map[syms.row-1].push_back(syms);
        row_to_sym_map[syms.row].push_back(syms);
        row_to_sym_map[syms.row+1].push_back(syms);
    }

    for(const auto& num: Context.numbers){
        for(const auto& sym: row_to_sym_map[num.row]){
            if(adjacent(sym,num)) {
                adjacencies[num].push_back(sym);
                if(sym.value == '*') adjacencies_2[sym].push_back(num);
            }
        }
    }
    
    long long answer = 0, answer_2 = 0;
    
    for(const auto& pairs: adjacencies) {
        answer += pairs.first.value;
    }
    
    for(const auto& pairs: adjacencies_2) {
        if(pairs.first.value == '*' && pairs.second.size() == 2) {
            answer_2 += pairs.second[0].value * pairs.second[1].value;
        }
    }

    std::cout << answer << '\n' << answer_2 << '\n';

    return 0;

}
