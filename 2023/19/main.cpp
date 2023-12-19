#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <stack>
#include <iomanip>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1;

    int64_t answer_1 = 0;

    for(const auto & p : Context.parts)
    {
        std::string workflow = "in";
        int64_t rule_no = 0;

        bool done = false, answer, result;

        while(!done) 
        {
            // have we reached the end?
            if(rule_no >= Context.workflows[workflow].size()) {
                done = true;
                answer = true;
            }

            // evaluate current rule 
            aoc::rule the_rule = Context.workflows[workflow][rule_no];

            int64_t lhs;

            switch(the_rule.type)
            {
                case 'A':
                    answer = true;
                    done = true;
                    break;

                case 'R':
                    answer = false;
                    done = true;
                    break;

                case 'N':
                    workflow = the_rule.name;
                    rule_no = 0;
                    break;

                case 'M':
                    switch(the_rule.letter)
                    {
                        case    'x':    lhs = p.x; break;
                        case    'm':    lhs = p.m; break;
                        case    'a':    lhs = p.a; break;
                        case    's':    lhs = p.s; break;
                        default:        std::cout << the_rule.letter << '\n'; assert(false);  
                    }

                    if(the_rule.compare == '<') 
                        result = lhs < the_rule.num;
                    else if(the_rule.compare == '>') 
                        result = lhs > the_rule.num;
                    else 
                        assert(false);

                    if(result) {    workflow = the_rule.name; rule_no = 0;  }
                    else    rule_no++;

                    break;

                case 'C':
                    switch(the_rule.letter)
                    {
                        case    'x':    lhs = p.x; break;
                        case    'm':    lhs = p.m; break;
                        case    'a':    lhs = p.a; break;
                        case    's':    lhs = p.s; break;
                        default:        std::cout << the_rule.letter << '\n'; assert(false);  
                    }

                    if(the_rule.compare == '<') 
                        result = lhs < the_rule.num;
                    else if(the_rule.compare == '>') 
                        result = lhs > the_rule.num;
                    else 
                        assert(false);

                    if(result) {    done = true; answer = the_rule.is_accept;  }
                    rule_no++;

                    break;
                default:    assert(false);  
            }
        }

        if(answer)  answer_1 += p.x + p.m + p.a + p.s;

    }

    std::cout << answer_1 << '\n';

    // part 2, why not brute force ? well it would take a billion years
    // so use interval

    std::vector<aoc::rating_set> accept, reject;

    std::stack<aoc::rating_set> in_flight;

    aoc::rating_set initial;
    initial.workflow = "in";
    initial.rule_no = 0;
    boost::icl::discrete_interval<int64_t> start_range(1,4001);
    initial.x = start_range;
    initial.m = start_range;
    initial.a = start_range;
    initial.s = start_range;

    in_flight.push(initial);

    while(!in_flight.empty())
    {
        aoc::rating_set current = in_flight.top(), matched_set, unmatched_set;
        in_flight.pop();

        // have we reached the end?
        if(current.rule_no >= Context.workflows[current.workflow].size()) {
            accept.push_back(current);
            continue;
        }

        // evaluate current rule 
        aoc::rule the_rule = Context.workflows[current.workflow][current.rule_no];

        boost::icl::interval_set<int64_t> matched, unmatched;
        boost::icl::discrete_interval<int64_t> lhs, match_set;

        switch(the_rule.type)
        {
            case 'A':
                accept.push_back(current);
                break;

            case 'R':
                reject.push_back(current);
                break;

            case 'N':
                current.workflow = the_rule.name;
                current.rule_no = 0;
                in_flight.push(current);
                break;

            case 'M':
                switch(the_rule.letter)
                {
                    case    'x':    lhs = current.x; break;
                    case    'm':    lhs = current.m; break;
                    case    'a':    lhs = current.a; break;
                    case    's':    lhs = current.s; break;
                    default:        std::cout << the_rule.letter << '\n'; assert(false);  
                }

                if(the_rule.compare == '<') {
                    match_set = boost::icl::discrete_interval<int64_t>(1,the_rule.num);
                }   else    {
                    match_set = boost::icl::discrete_interval<int64_t>(the_rule.num+1, 4001);
                }

                matched.insert(lhs & match_set);
                unmatched.insert(lhs);
                unmatched -= matched;

                for(const auto & m : matched) {

                    matched_set = current;
                
                    switch(the_rule.letter)
                    {
                        case    'x':    matched_set.x = m; break;
                        case    'm':    matched_set.m = m; break;
                        case    'a':    matched_set.a = m; break;
                        case    's':    matched_set.s = m; break;
                        default:        std::cout << the_rule.letter << '\n'; assert(false);  
                    }
                    
                    matched_set.workflow = the_rule.name;
                    matched_set.rule_no = 0;

                    in_flight.push(matched_set);

                }

                for(const auto & u : unmatched) {

                    unmatched_set = current;
                
                    switch(the_rule.letter)
                    {
                        case    'x':    unmatched_set.x = u; break;
                        case    'm':    unmatched_set.m = u; break;
                        case    'a':    unmatched_set.a = u; break;
                        case    's':    unmatched_set.s = u; break;
                        default:        std::cout << the_rule.letter << '\n'; assert(false);  
                    }

                    unmatched_set.rule_no++;

                    in_flight.push(unmatched_set);
                }

                break;

            case 'C':
                switch(the_rule.letter)
                {
                    case    'x':    lhs = current.x; break;
                    case    'm':    lhs = current.m; break;
                    case    'a':    lhs = current.a; break;
                    case    's':    lhs = current.s; break;
                    default:        std::cout << the_rule.letter << '\n'; assert(false);  
                }

                if(the_rule.compare == '<') {
                    match_set = boost::icl::discrete_interval<int64_t>(1,the_rule.num);
                }   else    {
                    match_set = boost::icl::discrete_interval<int64_t>(the_rule.num+1, 4001);
                }

                matched.insert(lhs & match_set);
                unmatched.insert(lhs);
                unmatched -= matched;

                for(const auto & m : matched) {

                    matched_set = current;
                
                    switch(the_rule.letter)
                    {
                        case    'x':    matched_set.x = m; break;
                        case    'm':    matched_set.m = m; break;
                        case    'a':    matched_set.a = m; break;
                        case    's':    matched_set.s = m; break;
                        default:        std::cout << the_rule.letter << '\n'; assert(false);  
                    }

                    if(the_rule.is_accept)
                        accept.push_back(matched_set);
                    else 
                        reject.push_back(matched_set);
                }

                for(const auto & u : unmatched) {

                    unmatched_set = current;
                
                    switch(the_rule.letter)
                    {
                        case    'x':    unmatched_set.x = u; break;
                        case    'm':    unmatched_set.m = u; break;
                        case    'a':    unmatched_set.a = u; break;
                        case    's':    unmatched_set.s = u; break;
                        default:        std::cout << the_rule.letter << '\n'; assert(false);  
                    }

                    unmatched_set.rule_no++;

                    in_flight.push(unmatched_set);
                }

                break;
            default:    assert(false);  
        }

    }

    int64_t answer_2 = 0;

    for(const auto& s : accept) {
        answer_2 += (s.x.upper() - s.x.lower() ) 
            * (s.m.upper() - s.m.lower() ) 
            * (s.a.upper() - s.a.lower() ) 
            * (s.s.upper() - s.s.lower() );
        
        if(false)
        std::cout << '[' << std::setw(4) << s.x.lower() << ',' << std::setw(4) << s.x.upper() << ") , "
                  << '[' << std::setw(4) << s.m.lower() << ',' << std::setw(4) << s.m.upper() << ") , "
                  << '[' << std::setw(4) << s.a.lower() << ',' << std::setw(4) << s.a.upper() << ") , "
                  << '[' << std::setw(4) << s.s.lower() << ',' << std::setw(4) << s.s.upper() << ") [ "
                  << s.workflow << ' ' << s.rule_no << " ]\n";
    }

    std::cout << answer_2 << '\n';

    return 0;
}

// 181822 too low