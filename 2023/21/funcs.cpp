#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}


auto neighbours(const std::vector<std::string>& field, const std::pair<int64_t,int64_t>& point) -> std::vector<std::pair<int64_t,int64_t>>
{
    std::vector<std::pair<int64_t,int64_t>> answer; 

    if(point.first > 0 && field[point.first-1][point.second] != '#')
        answer.push_back({point.first-1,point.second});
    if(point.second > 0 && field[point.first][point.second-1] != '#')
        answer.push_back({point.first,point.second-1});
    if(point.first < field.size()-1 && field[point.first+1][point.second] != '#')
        answer.push_back({point.first+1,point.second});
    if(point.second < field.size()-1 && field[point.first][point.second+1] != '#')
        answer.push_back({point.first,point.second+1});
    return answer; 
}

auto reached( 
    const std::map<std::pair<int64_t,int64_t>, std::vector<std::pair<int64_t,int64_t>>>& lookup_table,
    int64_t steps, 
    const std::pair<int64_t,int64_t>& start) -> std::set<std::pair<int64_t,int64_t>>
{
    std::set<std::pair<int64_t,int64_t>> answer, current;

    current.insert(start);

    for(int64_t i = 0; i < steps; ++i)
    {
        answer.clear();

        std::vector<std::pair<int64_t,int64_t>> item; 

        for(const auto & point : current) {
            item.clear();
            auto item_it = lookup_table.find(point);
            if(item_it != lookup_table.end()) {
                item = item_it->second;
            }
            for(const auto & it : item) {
                answer.insert(it);
            }
        }

        current = std::move(answer);
    }
    return current;
}


auto state_maker(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>
{
    std::vector<std::string> current = initial, next;

    int64_t answer_1 = 0;
    
    for(auto&& r : current) for(auto&& c : r) if (c == 'S') c = '.';

    std::map<std::vector<std::string>,std::vector<int>> states;

    for(int i = 0; i < imax; ++i) 
    {
        next = initial;

        if(i == 0) 
        {
            for(int y = 0; y < current.size(); ++y)
            {
                for(int x = 0; x < current[y].size(); ++x)
                {
                    if(initial[y][x] == 'S')
                    {
                        if(y>0) next[y-1][x] = next[y-1][x] == '.' ? 'O' : '#';
                        if(y < next.size() - 1) next[y+1][x] = next[y+1][x] == '.' ? 'O' : '#';
                        if(x>0) next[y][x-1] = next[y][x-1] == '.' ? 'O' : '#';
                        if(x < next[y].size() - 1)next[y][x+1] = next[y][x+1] == '.' ? 'O' : '#';
                    }
                }
            }
        }
        else {
            for(int y = 0; y < current.size(); ++y)
            {
                for(int x = 0; x < current[y].size(); ++x)
                {
                    if(current[y][x] == '#') continue;
                    if(y > 0)
                        if(current[y-1][x] == 'O')
                            next[y][x] = 'O';
                    if(x > 0)
                        if(current[y][x-1] == 'O')
                            next[y][x] = 'O';
                    if(y < current.size() - 1)
                        if(current[y+1][x] == 'O')
                            next[y][x] = 'O';
                    if(x < current[y].size() - 1)
                        if(current[y][x+1] == 'O')
                            next[y][x] = 'O';
                }
            }
        }

        current = next;

        states[current].push_back(i);
    }

    return states;
}


auto state_maker_top_left(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>
{
    std::vector<std::string> current = initial, next;

    int64_t answer_1 = 0;
    
    for(auto&& r : current) for(auto&& c : r) if (c == 'S') c = '.';

    std::map<std::vector<std::string>,std::vector<int>> states;

    for(int i = 0; i < imax; ++i) 
    {
        next = initial;

        for(int y = 0; y < current.size(); ++y)
        {
            for(int x = 0; x < current[y].size(); ++x)
            {
                if(current[y][x] == '#') continue;
                if(y > 0)
                    if(current[y-1][x] == 'O')
                        next[y][x] = 'O';
                if(x > 0)
                    if(current[y][x-1] == 'O')
                        next[y][x] = 'O';
                if(y < current.size() - 1)
                    if(current[y+1][x] == 'O')
                        next[y][x] = 'O';
                if(x < current[y].size() - 1)
                    if(current[y][x+1] == 'O')
                        next[y][x] = 'O';

                if(y == 0 && x <= i && (x+i) % 2 == 0)
                    next[y][x] = 'O';

                if(x == 0 && y <= i && (y+i) % 2 == 0)
                    next[y][x] = 'O';
                    
            }
        }

        current = next;

        states[current].push_back(i);
    }

    return states;
}

auto state_maker_top_right(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>
{
    std::vector<std::string> current = initial, next;

    int64_t answer_1 = 0;
    
    for(auto&& r : current) for(auto&& c : r) if (c == 'S') c = '.';

    std::map<std::vector<std::string>,std::vector<int>> states;

    for(int i = 0; i < imax; ++i) 
    {
        next = initial;

        for(int y = 0; y < current.size(); ++y)
        {
            for(int x = 0; x < current[y].size(); ++x)
            {
                if(current[y][x] == '#') continue;
                if(y > 0)
                    if(current[y-1][x] == 'O')
                        next[y][x] = 'O';
                if(x > 0)
                    if(current[y][x-1] == 'O')
                        next[y][x] = 'O';
                if(y < current.size() - 1)
                    if(current[y+1][x] == 'O')
                        next[y][x] = 'O';
                if(x < current[y].size() - 1)
                    if(current[y][x+1] == 'O')
                        next[y][x] = 'O';

                if(y == 0 && next[y].size() -1 - x <= i && (x+i) % 2 == 0)
                    next[y][x] = 'O';

                if(x == next[y].size()-1 && y <= i && (y+i) % 2 == 0)
                    next[y][x] = 'O';
                    
            }
        }

        current = next;

        states[current].push_back(i);
    }

    return states;
}

auto state_maker_bottom_right(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>
{
    std::vector<std::string> current = initial, next;

    int64_t answer_1 = 0;
    
    for(auto&& r : current) for(auto&& c : r) if (c == 'S') c = '.';

    std::map<std::vector<std::string>,std::vector<int>> states;

    for(int i = 0; i < imax; ++i) 
    {
        next = initial;

        for(int y = 0; y < current.size(); ++y)
        {
            for(int x = 0; x < current[y].size(); ++x)
            {
                if(current[y][x] == '#') continue;
                if(y > 0)
                    if(current[y-1][x] == 'O')
                        next[y][x] = 'O';
                if(x > 0)
                    if(current[y][x-1] == 'O')
                        next[y][x] = 'O';
                if(y < current.size() - 1)
                    if(current[y+1][x] == 'O')
                        next[y][x] = 'O';
                if(x < current[y].size() - 1)
                    if(current[y][x+1] == 'O')
                        next[y][x] = 'O';

                if(y == next.size()-1 && next[y].size() -1 - x <= i && (x+i) % 2 == 0)
                    next[y][x] = 'O';

                if(x == next[y].size()-1 && next.size() - 1 - y <= i && (y+i) % 2 == 0)
                    next[y][x] = 'O';
                    
            }
        }

        current = next;

        states[current].push_back(i);
    }

    return states;
}

auto state_maker_bottom_left(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>
{
    std::vector<std::string> current = initial, next;

    int64_t answer_1 = 0;
    
    for(auto&& r : current) for(auto&& c : r) if (c == 'S') c = '.';

    std::map<std::vector<std::string>,std::vector<int>> states;

    for(int i = 0; i < imax; ++i) 
    {
        next = initial;

        for(int y = 0; y < current.size(); ++y)
        {
            for(int x = 0; x < current[y].size(); ++x)
            {
                if(current[y][x] == '#') continue;
                if(y > 0)
                    if(current[y-1][x] == 'O')
                        next[y][x] = 'O';
                if(x > 0)
                    if(current[y][x-1] == 'O')
                        next[y][x] = 'O';
                if(y < current.size() - 1)
                    if(current[y+1][x] == 'O')
                        next[y][x] = 'O';
                if(x < current[y].size() - 1)
                    if(current[y][x+1] == 'O')
                        next[y][x] = 'O';

                if(y == next.size()-1 && x <= i && (x+i) % 2 == 0)
                    next[y][x] = 'O';

                if(x == 0 && next.size() - 1 - y <= i && (y+i) % 2 == 0)
                    next[y][x] = 'O';
                    
            }
        }

        current = next;

        states[current].push_back(i);
    }

    return states;
}