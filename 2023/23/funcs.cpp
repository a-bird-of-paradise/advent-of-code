#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <ranges>

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto get_index(int64_t x, int64_t y) -> int64_t
{
    return 10'000*x + y;
}

auto build_graph(const std::vector<std::string>& field) -> std::map<int64_t, std::vector<int64_t>>
{

    std::map<int64_t, std::vector<int64_t>> graph;

    for(int64_t y = 0; y < field.size(); ++y) {
        for(int64_t x = 0; x < field[y].size(); ++x) {

            switch(field[y][x])
            {
                case '#':
                    break;
                case '.':

                    if(y>0 && field[y-1][x] != '#' && field[y-1][x] != 'v') {
                        graph[get_index(x,y)].push_back(get_index(x,y-1));
                    }

                    if(y<field.size()-1 && field[y+1][x] != '#' && field[y+1][x] != '^') {
                        graph[get_index(x,y)].push_back(get_index(x,y+1));
                    }

                    if(x>0 && field[y][x-1] != '#' && field[y][x-1] != '>') {
                        graph[get_index(x,y)].push_back(get_index(x-1,y));
                    }

                    if(x<field[y].size()-1 && field[y][x+1] != '#' && field[y][x+1] != '<') {
                        graph[get_index(x,y)].push_back(get_index(x+1,y));
                    }

                    break;
                case '<':

                    graph[get_index(x,y)].push_back(get_index(x-1,y));

                    break;
                case '>':

                    graph[get_index(x,y)].push_back(get_index(x+1,y));
                    
                    break;
                case 'v':

                    graph[get_index(x,y)].push_back(get_index(x,y+1));
                    
                    break;
                case '^':

                    graph[get_index(x,y)].push_back(get_index(x,y-1));
                    
                    break;
                default:    assert(false);
            }

        }
    }
    
    return graph;

}

auto build_graph_2(const std::vector<std::string>& field) -> std::map<int64_t, std::vector<int64_t>>
{

    std::map<int64_t, std::vector<int64_t>> graph;

    for(int64_t y = 0; y < field.size(); ++y) {
        for(int64_t x = 0; x < field[y].size(); ++x) {

            switch(field[y][x])
            {
                case '#':
                    break;
                case '.':
                case '<':
                case '>':
                case 'v':
                case '^':

                    if(y>0 && field[y-1][x] != '#') {
                        graph[get_index(x,y)].push_back(get_index(x,y-1));
                    }

                    if(y<field.size()-1 && field[y+1][x] != '#') {
                        graph[get_index(x,y)].push_back(get_index(x,y+1));
                    }

                    if(x>0 && field[y][x-1] != '#') {
                        graph[get_index(x,y)].push_back(get_index(x-1,y));
                    }

                    if(x<field[y].size()-1 && field[y][x+1] != '#') {
                        graph[get_index(x,y)].push_back(get_index(x+1,y));
                    }

                    break;
                default:    assert(false);
            }

        }
    }
    
    return graph;

}

auto dfs(
    const std::map<int64_t, std::vector<int64_t>>& graph,
    const int64_t goal,
    const int64_t current,
    std::vector<int64_t>& current_path,
    std::vector<std::vector<int64_t>>& paths) -> void
{
    // we're here
    current_path.push_back(current);

    // at the end?
    if(current == goal) {
        paths.push_back(current_path);
        current_path.pop_back();
        return;
    }

    // try each neighbour 

    for(const auto& n : graph.at(current))
    {
        if(std::ranges::find(current_path,n) == current_path.end()) {
            dfs(graph,goal,n,current_path,paths);
        }
    }

    current_path.pop_back();
    return;
}

auto compact(
    const std::map<int64_t, std::vector<int64_t>>& graph,
    int64_t start,
    int64_t end) -> std::map<int64_t, std::vector<std::pair<int64_t,int64_t>>>
{
    std::map<int64_t, std::vector<std::pair<int64_t,int64_t>>> answer;

    std::vector<int64_t> intersections;

    intersections.push_back(start);

    for(const auto& g : graph) {
        if(g.second.size() > 2) {
            intersections.push_back(g.first);
        }
    }

    intersections.push_back(end);

    int64_t distance, previous, next, current;

    bool dead_end;

    for(const auto& i : intersections)
    {
        for(const auto & n : graph.at(i))
        {
            dead_end = false;
            previous = i;
            distance = 1;
            current = n; 
            // seek along path until we bump into an intersection 
            
            while(std::ranges::find(intersections,current) == intersections.end()) // so current always has 2 neihbours
            {
                if(graph.at(current).size() != 2) {
                    dead_end = true;
                    std::cout << i << ' ' << current << ' ' << '\n';
                     break; 
                }// ... except at dead ends

                if(graph.at(current)[0] == previous) {
                    next = graph.at(current)[1];
                } else {
                    next = graph.at(current)[0];
                }

                previous = current;
                current = next; 
                distance++;
            }
            
            if(!dead_end) 
                answer[i].push_back({current,distance});
        }
    }
    return answer;
}
