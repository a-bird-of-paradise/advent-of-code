#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <map>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    int64_t answer_1 = 0;
    std::vector<std::vector<int64_t>> valid_tickets;

    for(const auto& ticket : Context.nearby_tickets)
    {
        bool valid_ticket = true;
        for(std::size_t field_num = 0; field_num < ticket.size(); ++field_num)
        {
            bool seen = false;
            for(std::size_t range_num = 0; range_num < Context.ranges.size(); ++range_num)
            {
                if(boost::icl::contains(Context.ranges[range_num],ticket[field_num])) {
                    seen = true;
                }
            }
            if(!seen) {
                answer_1 += ticket[field_num];
                valid_ticket = false;
            }
        }
        if(valid_ticket)
        {
            valid_tickets.push_back(ticket);
        }
    }

    std::cout << answer_1 << '\n';

    // part 2

    std::vector<std::vector<bool>> could_be, increment; // could field [i] be range [j]?
    std::vector<std::size_t> lookup;
    lookup.resize(Context.ticket.size());

    std::map<std::string,std::size_t> name_to_field;

    // enlarge to size
    could_be.resize(Context.ticket.size());
    for(auto&& x : could_be) x.resize(Context.ranges.size());

    // set true
    for(auto&& x : could_be) for(auto&& y : x) y = true;
    increment = could_be; // copy 

    for(const auto& ticket : valid_tickets)
        for(std::size_t field_num = 0; field_num < ticket.size(); ++field_num)
            for(std::size_t range_num = 0; range_num < Context.ranges.size(); ++range_num)
                if(!boost::icl::contains(Context.ranges[range_num],ticket[field_num]))
                    could_be[field_num][range_num] = false;

    for(std::size_t i = 0; i < could_be.size(); ++i)
    {
        int64_t counter = 0;

        for(std::size_t j = 0; j < could_be[i].size(); ++j)
            if(could_be[i][j]) 
                counter++; 

        lookup[counter-1]=i;
    }

    for(std::size_t i = 0; i < could_be.size(); ++i)
        for(std::size_t j = 0; j < could_be[i].size(); ++j)
            if(i == 0) 
                increment[i][j] = could_be[lookup[i]][j];
            else 
                increment[i][j] = (could_be[lookup[i]][j]) ^ (could_be[lookup[i-1]][j]);

    for(std::size_t i = 0; i < could_be.size(); ++i)
        for(std::size_t j = 0; j < could_be[i].size(); ++j)
            if(increment[i][j]) 
                name_to_field[Context.names[j]] = lookup[i];

    uint64_t answer_2 = 1;

    for(const auto& p : name_to_field) 
        if(p.first.starts_with("departure"))
            answer_2 *= Context.ticket[p.second];
    std::cout << answer_2 << '\n';

    return 0;
}
