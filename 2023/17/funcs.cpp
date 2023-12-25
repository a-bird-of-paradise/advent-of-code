#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <queue>
#include <map>

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto inline heuristic_fn(const aoc::point& v, const aoc::point& goal) -> int64_t
{
    return std::abs(v.c - goal.c) + std::abs(v.r - goal.r);
}

auto part_1(const aoc::context& Context) -> void
{

    // part 1 

    std::map<aoc::state,int64_t> costs;
    std::map<aoc::state,aoc::state> previous;

    aoc::point last_point(Context.field.size()-1,Context.field[0].size()-1);
    aoc::state last_state;

    auto cmp = [&](const auto & left, const auto & right){ 
        return costs[right] + heuristic_fn(right.p,last_point) < costs[left] + heuristic_fn(left.p,last_point); 
    };
    std::priority_queue<aoc::state, std::vector<aoc::state>, decltype(cmp)> unvisited(cmp);

    // put first possible states in

    unvisited.push({0,1,aoc::direction::right}); 
    costs[{0,1,aoc::direction::right}]=Context.field[0][1];

    unvisited.push({0,2,aoc::direction::right});
    costs[{0,2,aoc::direction::right}]=Context.field[0][1]+Context.field[0][2];

    unvisited.push({0,3,aoc::direction::right});
    costs[{0,3,aoc::direction::right}]=Context.field[0][1]+Context.field[0][2]+Context.field[0][3];

    unvisited.push({1,0,aoc::direction::down});
    costs[{1,0,aoc::direction::down}]=Context.field[1][0];

    unvisited.push({2,0,aoc::direction::down});
    costs[{2,0,aoc::direction::down}]=Context.field[1][0]+Context.field[2][0];

    unvisited.push({3,0,aoc::direction::down});
    costs[{3,0,aoc::direction::down}]=Context.field[1][0]+Context.field[2][0]+Context.field[3][0];

    while(!unvisited.empty())
    {
        aoc::state current = unvisited.top();
        unvisited.pop();

        if(current.p == last_point) { last_state = current; break; }

        switch(current.dir)
        {
            int64_t cost_counter;
            case aoc::direction::up:
            case aoc::direction::down:
                // left
                cost_counter = 0;
                for(int64_t i = current.p.c-1; i >= std::max(0LL,current.p.c-3); --i)
                {
                    cost_counter += Context.field[current.p.r][i];
                    aoc::state next{current.p.r,i,aoc::direction::left};

                    int64_t new_cost = costs[current] + cost_counter;
                    if(!costs.contains(next)) {
                        costs[next] = INT64_MAX;
                    }
                    if(new_cost < costs[next]) {
                        costs[next] = new_cost;
                        previous[next] = current;
                        unvisited.push(next);
                    }
                }
                // right
                cost_counter = 0;
                for(int64_t i = current.p.c+1; i <= std::min<int64_t>(Context.field[0].size()-1, current.p.c+3LL); ++i)
                {
                    cost_counter += Context.field[current.p.r][i];
                    aoc::state next{current.p.r,i,aoc::direction::right};

                    int64_t new_cost = costs[current] + cost_counter;
                    if(!costs.contains(next)) {
                        costs[next] = INT64_MAX;
                    }
                    if(new_cost < costs[next]) {
                        costs[next] = new_cost;
                        previous[next] = current;
                        unvisited.push(next);
                    }
                }
                break;
            case aoc::direction::left:
            case aoc::direction::right:
                // up
                cost_counter = 0;
                for(int64_t i = current.p.r-1; i >= std::max(0LL,current.p.r-3); --i)
                {
                    cost_counter += Context.field[i][current.p.c];
                    aoc::state next{i,current.p.c,aoc::direction::up};

                    int64_t new_cost = costs[current] + cost_counter;
                    if(!costs.contains(next)) {
                        costs[next] = INT64_MAX;
                    }
                    if(new_cost < costs[next]) {
                        costs[next] = new_cost;
                        previous[next] = current;
                        unvisited.push(next);
                    }
                }
                // down
                cost_counter = 0;
                for(int64_t i = current.p.r+1; i <= std::min<int64_t>(Context.field.size()-1, current.p.r+3LL); ++i)
                {
                    cost_counter += Context.field[i][current.p.c];
                    aoc::state next{i,current.p.c,aoc::direction::down};

                    int64_t new_cost = costs[current] + cost_counter;
                    if(!costs.contains(next)) {
                        costs[next] = INT64_MAX;
                    }
                    if(new_cost < costs[next]) {
                        costs[next] = new_cost;
                        previous[next] = current;
                        unvisited.push(next);
                    }
                }

                break;
        }
    }

    std::cout << costs[last_state] << '\n'; 
}


auto part_2(const aoc::context& Context) -> void
{
    // part 2

    std::map<aoc::state,int64_t> costs;
    std::map<aoc::state,aoc::state> previous;
    
    aoc::point last_point(Context.field.size()-1,Context.field[0].size()-1);
    aoc::state last_state;

    auto cmp = [&](const auto & left, const auto & right){ 
        return costs[right] + heuristic_fn(right.p,last_point) < costs[left] + heuristic_fn(left.p,last_point); 
    };
    std::priority_queue<aoc::state, std::vector<aoc::state>, decltype(cmp)> unvisited(cmp);

    // put first possible states in

    int64_t rcount,ccount;
    ccount = Context.field[1][0]+ Context.field[2][0]+ Context.field[3][0];
    rcount = Context.field[0][1]+ Context.field[0][2]+ Context.field[0][3];

    for(int i = 4; i <= 10; i++) {
        unvisited.push({0,i,aoc::direction::right});
        unvisited.push({i,0,aoc::direction::down});
        rcount += Context.field[0][i];
        ccount += Context.field[i][0];
        costs[{0,i,aoc::direction::right}] = rcount;
        costs[{i,0,aoc::direction::down}] = ccount;
    }

    while(!unvisited.empty())
    {
        aoc::state current = unvisited.top();
        unvisited.pop();

        if(current.p == last_point) { last_state = current; break; }

        switch(current.dir)
        {
            int64_t cost_counter, skipper;
            case aoc::direction::up:
            case aoc::direction::down:
                // left
                cost_counter = 0;
                skipper = 0;
                if(current.p.c-4 >= 0) {
                    for(int64_t i = current.p.c-1; i >= std::max(0LL,current.p.c-10); --i)
                    {
                        cost_counter += Context.field[current.p.r][i];
                        aoc::state next{current.p.r,i,aoc::direction::left};

                        if(!costs.contains(next)) {
                            costs[next] = INT64_MAX;
                        }

                        skipper++;

                        if(skipper>=4) {
                            int64_t new_cost = costs[current] + cost_counter;
                            if(new_cost < costs[next]) {
                                costs[next] = new_cost;
                                previous[next] = current;
                                unvisited.push(next);
                            }
                        }
                    }
                }
                // right
                cost_counter = 0;
                skipper = 0;
                if(current.p.c+4 <Context.field[0].size()) {
                    for(int64_t i = current.p.c+1; i <= std::min<int64_t>(Context.field[0].size()-1, current.p.c+10LL); ++i)
                    {
                        cost_counter += Context.field[current.p.r][i];
                        aoc::state next{current.p.r,i,aoc::direction::right};
                        
                        if(!costs.contains(next)) {
                            costs[next] = INT64_MAX;
                        }

                        skipper++;

                        if(skipper>=4) {
                            int64_t new_cost = costs[current] + cost_counter;
                            if(new_cost < costs[next]) {
                                costs[next] = new_cost;
                                previous[next] = current;
                                unvisited.push(next);
                            }
                        }
                    }
                }
                break;
            case aoc::direction::left:
            case aoc::direction::right:
                // up
                cost_counter = 0;
                skipper = 0;
                if(current.p.r-4>=0) {
                    for(int64_t i = current.p.r-1; i >= std::max(0LL,current.p.r-10); --i)
                    {
                        cost_counter += Context.field[i][current.p.c];
                        aoc::state next{i,current.p.c,aoc::direction::up};
                        
                        if(!costs.contains(next)) {
                            costs[next] = INT64_MAX;
                        }

                        skipper++;

                        if(skipper>=4) {
                            int64_t new_cost = costs[current] + cost_counter;
                            if(new_cost < costs[next]) {
                                costs[next] = new_cost;
                                previous[next] = current;
                                unvisited.push(next);
                            }
                        }
                    }
                }
                // down
                cost_counter = 0;
                skipper = 0;
                if(current.p.r+4 < Context.field.size()) {
                    for(int64_t i = current.p.r+1; i <= std::min<int64_t>(Context.field.size()-1, current.p.r+10LL); ++i)
                    {
                        cost_counter += Context.field[i][current.p.c];
                        aoc::state next{i,current.p.c,aoc::direction::down};
                        
                        if(!costs.contains(next)) {
                            costs[next] = INT64_MAX;
                        }

                        skipper++;

                        if(skipper>=4) {
                            int64_t new_cost = costs[current] + cost_counter;
                            if(new_cost < costs[next]) {
                                costs[next] = new_cost;
                                previous[next] = current;
                                unvisited.push(next);
                            }
                        }
                    }
                }
            break;
        }
    }
    std::cout << costs[last_state] << '\n'; 
}