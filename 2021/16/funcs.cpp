#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

unsigned long long aoc::packet::eval(void)
{
    unsigned long long answer;
    switch(type_id) {
    case 0:
        // sum
        answer = 0;
        for(auto & i : children) answer += i.eval();
        break;
    case 1:
        // product
        answer = 1;
        for(auto & i : children) answer *= i.eval();
        break;
    case 2:
        // min
        answer = UINT64_MAX;
        for(auto & i : children) answer = i.eval() < answer ? i.eval() : answer;
        break;
    case 3:
        // max
        answer = 0;
        for(auto & i : children) answer = i.eval() > answer ? i.eval() : answer;
        break;
    case 4:
        // data
        answer = data_value;
        break;
    case 5:
        // greater than
        if(children[0].eval() > children[1].eval()) 
            answer = 1;
        else
            answer = 0;
        break;
    case 6:
        // less than
        if(children[0].eval() < children[1].eval()) 
            answer = 1;
        else
            answer = 0;
        break;
    case 7:
        // equals
        if(children[0].eval() == children[1].eval()) 
            answer = 1;
        else
            answer = 0;
        break;
    default:
        answer = 0;
        std::cout << "INVALID PACKET\n";
    }
    return answer;
}


aoc::parser::symbol_type aoc::scanner::pop_token(aoc::context& Context){

    if(!Context.terminator_counter.empty()) {
        std::pair<int,long long> top = Context.terminator_counter.top();

        if(top.first == 0) {
            if(top.second == Context.bit_counter) {
                Context.terminator_counter.pop();
                return aoc::parser::make_TERMINATOR(Context.loc); 
            }

        } else {
            if(top.second == 0) {
                Context.terminator_counter.pop();
                return aoc::parser::make_TERMINATOR(Context.loc); 
            }
        }
    }

    int token = Context.tokens.front();
    Context.tokens.pop();

    switch (token) {
        case 0:
            Context.bit_counter++;
            return aoc::parser::make_ZERO(Context.loc); 
            break;
        case 1: 
            Context.bit_counter++;
            return aoc::parser::make_ONE(Context.loc); 
            break;
        case 2: 
            return aoc::parser::make_TERMINATOR(Context.loc); 
            break;
        default: return aoc::parser::make_YYUNDEF(Context.loc);
    }
}