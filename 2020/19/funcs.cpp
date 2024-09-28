#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <ranges>

auto match(
    std::string_view to_match, 
    std::stack<int> states,
    const std::map<int, aoc::rule>& rules) -> bool
{
    // end case: consumed all the string
    // ... but still states left : FAIL
    if(to_match.size() == 0 && states.size() > 0) return false;
    // and inverse
    if(to_match.size() > 0 && states.size() == 0) return false; 
    // success case
    if(to_match.size() == 0 && states.size() == 0) return true;
    // if more states than characters then also fault 
    if(to_match.size() < states.size()) return false;

    // ok. Is the front state a terminal? If so does it match? 

    auto front_rule = rules.find(states.top());

    if(front_rule->second.type == aoc::rule_type::terminal)
    {
        if(to_match[0] != front_rule->second.c) return false; 
        states.pop(); // consume the terminal
        return match(to_match.substr(1),states,rules); 
    }

    // if we are here then the front is a nonterminal 
    // so for each of its sub rules
    // ... pop stack
    // ... push subrule onto it 
    // ... recurse 

    for(const auto & subrule : front_rule->second.rules)
    {
        std::stack<int> new_states = states;
        new_states.pop();
        for(const auto & state : subrule | std::views::reverse)
            new_states.push(state);
        if(match(to_match,new_states,rules)) return true;
    }

    return false;
}