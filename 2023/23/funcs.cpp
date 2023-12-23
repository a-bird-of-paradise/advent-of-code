#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

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