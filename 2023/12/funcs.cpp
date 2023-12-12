#include "funcs.hpp"
#include "scanner.hpp"
#include <algorithm>
#include "context.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto generate_candidates(const std::vector<char>& current,
                        std::size_t level,
                        std::set<std::vector<char>>& candidates) -> void
{

    if(level > current.size()) {
        candidates.insert(current);
        return; 
    }
    if(current[level] != '?') {
        generate_candidates(current,level+1,candidates);
    } else {

        std::vector<char> first = current, second = current;

        first[level] = '.';
        second[level] = '#';

        generate_candidates(first,level+1,candidates);
        generate_candidates(second,level+1,candidates);
    }

}

auto is_valid_candidate(const std::vector<char>& candidate, 
                        const std::vector<int64_t>& report) -> bool
{
    enum class state {INITIAL, DOT, HASH};

    std::vector<int64_t> seen, temp;

    state current_state = state::INITIAL;
    auto it = candidate.begin();
    int64_t current_length = 0;

    while(it != candidate.end())
    {
        char current = *it;

        switch(current_state)
        {
            case state::INITIAL:
                if(current == '#') {
                    current_state = state::HASH; 
                    current_length++;
                } else {
                    current_state = state::DOT;
                }
                break;
            
            case state::HASH:
                if(current == '#') {
                    current_length++;
                } else {
                    current_state = state::DOT;
                    seen.push_back(current_length);
                    current_length = 0;
                }
                break;

            case state::DOT:
                if(current == '#') {
                    current_length = 1;
                    current_state = state::HASH;
                } else {
                    // do nothing
                }
                break;
        }
        ++it;
    }

    if(current_state == state::HASH) seen.push_back(current_length);

    /*temp = seen;

    if(seen == report) {
        for(const auto & c : candidate) std::cout << c;
        std::cout << " | ";
        for(const auto & v : report) std::cout << v << ' ';
        std::cout << " | ";
        for(const auto & s : temp) std::cout << s << ' ';
        std::cout << '\n';
    }*/

    return seen == report;
}

