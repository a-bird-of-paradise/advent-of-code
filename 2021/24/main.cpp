#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <iomanip>
#include <set>
#include <algorithm>

int main (int argc, char **argv)
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    std::string input = "92793949489995";

    for(auto c = input.rbegin(); c != input.rend(); c++) {
        Context.input.push(*c - '0');
    }

    Parser.parse();

    for(const auto & a : Context.A) std::cout << std::setw(4) << a << ' ';
    std::cout << std::endl;
    for(const auto & a : Context.B) std::cout << std::setw(4) << a << ' ';
    std::cout << std::endl;
    for(const auto & a : Context.d) std::cout << std::setw(4) << a << ' ';
    std::cout << std::endl;

    std::cout << Context.A.size() << std::endl;

 
    std::vector<long long> current_number;
    std::set<std::vector<long long>, cmp> answers;
    std::set<long long> real_answers;

    dfs(Context,current_number,answers,14,0);

    long long current;

    for(const auto& i : answers) {
        current = 0;
        for(auto j = i.rbegin(); j != i.rend(); j++) {
            current *= 10;
            current += *j;
        }
        real_answers.insert(current);
    }

    std::cout << std::setw(14) << std::setfill('0') 
        << *std::max_element(real_answers.begin(), real_answers.end()) << std::endl;
    std::cout << std::setw(14) << std::setfill('0') 
        << *std::min_element(real_answers.begin(), real_answers.end()) << std::endl;

    return 0;

}
