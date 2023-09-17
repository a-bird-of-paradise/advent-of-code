#include <algorithm>
#include <iomanip>
#include <ranges>
#include <map>
#include <set> 
#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    /* part 1 */

    std::string answer = Context.Template;
    for(int i = 0; i < 10; i++)  answer = apply_rules(answer, Context.rules);

    std::cout << score_1(answer) << std::endl;    

    /* part 2 needs a different approach ... 
    use the rules to create generators and just iterate over that */

    // construct alphabet
    // cartesian product
    // test if in ruleset 
    
    std::set<char> alphabet;
    std::set<std::pair<char,char>> pairs;

    for(auto & i : Context.Template) alphabet.insert(i);

    for(auto & j : Context.rules) {
        alphabet.insert(std::get<0>(j));
        alphabet.insert(std::get<1>(j));
        alphabet.insert(std::get<2>(j));
    }

    for(auto & i : alphabet)
        for(auto & j : alphabet)
            pairs.insert(std::make_pair(i,j));

    std::map< std::pair<char,char>,
        std::map< std::pair<char,char>, long long>> generator;

    for(auto & i : pairs)
        for(auto & j : pairs)
            generator[i][j] = i == j ? 1 : 0;

    for(auto & i : Context.rules){
        std::pair<char,char> match, out_1, out_2;

        match = std::make_pair(std::get<0>(i),std::get<1>(i));
        out_1 = std::make_pair(std::get<0>(i),std::get<2>(i));
        out_2 = std::make_pair(std::get<2>(i),std::get<1>(i));

        generator[match][match] = 0;
        generator[match][out_1] += 1;
        generator[match][out_2] += 1;
    }

    std::map<std::pair<char,char>,long long> pair_counter, next_pair_counter;

    for(auto & i : pairs) pair_counter[i] = next_pair_counter[i] = 0;

    for(std::size_t i = 1; i < Context.Template.size(); i++) {
        pair_counter[std::make_pair(Context.Template[i-1],Context.Template[i])]++;
    }

    for(int i = 1; i <= 40; i++) {

        for(auto & outer : pairs) {
            for(auto & inner : pairs) {
                next_pair_counter[inner] += pair_counter[outer] * generator[outer][inner];
            }
        }

        pair_counter = next_pair_counter;

        for(auto & i : pairs) next_pair_counter[i]=0;
    }

    std::map<char,long long> histogram;
    for(auto & i : pairs) {
        histogram[i.first] += pair_counter[i];
        histogram[i.second] += pair_counter[i];
    }
    histogram[Context.Template[0]]++;
    histogram[Context.Template.back()]++;
    long long max = INT64_MIN, min = INT64_MAX;
    for(auto i : histogram) {
        max = i.second > max ? i.second : max;
        min = i.second < min ? i.second : min;
    }
    std::cout<< (max-min)/2 << std::endl;

    return 0;
}