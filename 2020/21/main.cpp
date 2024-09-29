#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <algorithm>
#include <map>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();
 
    std::set<std::string> all_allergens;
    for(const auto & line : Context.lines) 
        for(const auto & right: line.second)
            all_allergens.insert(right);

    std::map<std::string, std::set<std::string>> candidates;
    std::map<std::string, std::string> definites;

    // large list of candidates: any ingredient ever in a food with the allergen

    for(const auto & line : Context.lines) 
        for(const auto & allergen : all_allergens) 
            if(line.second.find(allergen) != line.second.end()) 
                candidates[allergen].insert(std::begin(line.first), std::end(line.first));

    // restrict candidates to those ingredients in every food

    for(const auto & line : Context.lines) {
        for(const auto & allergen : line.second) {
            std::set<std::string> orig = candidates[allergen], to_insert;

            std::set_intersection(
                std::begin(orig), 
                std::end(orig),
                std::begin(line.first),
                std::end(line.first),
                std::inserter(to_insert,to_insert.begin())
                );

            candidates[allergen] = to_insert;
        } 
    }

    std::set<std::string> ignore;

    for(const auto & x : candidates) 
        for(const auto & y : x.second) 
            ignore.insert(y);

    int64_t answer_1 = 0;

    for(const auto & line : Context.lines) 
        for(const auto & ingredient : line.first) 
            if(ignore.find(ingredient) == ignore.end()) 
                answer_1++;

    std::cout << answer_1 << std::endl;

    // prune candidates: any allergen with exactly one ingredient is solved, so put it into the definite map, remove from candidate, 
    // and remove ingredient from other candidates. loop until all candidates have been solved

    while(candidates.size() > 0) 
    {
        std::string filter_ingredient = "";
        std::map<std::string, std::set<std::string>>::iterator it;
        // seek to first solved
        for(it = candidates.begin(); it != candidates.end(); ++it) {
            if(it->second.size() == 1) {
                filter_ingredient = *(it->second.begin());
                definites[it->first] = filter_ingredient;
                candidates.erase(it);
                break; // iterator invalidated so break 
            }
        }

        for(auto && x : candidates) {
            std::erase_if(x.second, [&](auto const &x){ return x == filter_ingredient; });
        }
    }

    std::string answer_2;

    for(const auto & x : definites) {
        answer_2 += x.second;
        answer_2 += ',';
    }

    answer_2.pop_back();

    std::cout << answer_2 << std::endl;

    return 0;
}