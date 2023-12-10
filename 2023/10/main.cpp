#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <stack>
#include <queue>
#include <stack>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    aoc::point start;

    std::stack<aoc::point> unvisited;
    std::map<aoc::point,std::vector<aoc::point>> graph;

    for(const auto& p : Context.pipes)
        if(p.second == 'S')
            start = p.first;

    unvisited.push(start); 

    while(!unvisited.empty())
    {
        // get current
        aoc::point current = unvisited.top();
        unvisited.pop();
        if(graph.find(current) != graph.end()) continue;

        // seek neighbours

        char N,E,S,W;
        N = '.';
        E = '.';
        W = '.';
        S = '.';
        std::map<aoc::point,char>::iterator location;
        location = Context.pipes.find(aoc::point(current.x,current.y-1));
        if(location != Context.pipes.end()) N = location->second; else N = '.';

        location = Context.pipes.find(aoc::point(current.x,current.y+1));
        if(location != Context.pipes.end()) S = location->second; else S = '.';

        location = Context.pipes.find(aoc::point(current.x+1,current.y));
        if(location != Context.pipes.end()) E = location->second; else E = '.';

        location = Context.pipes.find(aoc::point(current.x-1,current.y));
        if(location != Context.pipes.end()) W = location->second; else W = '.';

        bool ex_N = false, ex_E= false, ex_S= false, ex_W= false;
        char current_c = Context.pipes[current];

        if(current_c == 'F' || current_c == '7' || current_c == '-') ex_N = true;
        if(current_c == 'J' || current_c == '7' || current_c == '|') ex_E = true;
        if(current_c == 'F' || current_c == 'L' || current_c == '|') ex_W = true;
        if(current_c == 'L' || current_c == 'J' || current_c == '-') ex_S = true;

        // add reachable neighbours
        //      N
        if((N == '|' || N == 'S' || N == 'F' || N == '7') && !ex_N){
            graph[current].push_back(aoc::point(current.x,current.y-1));
        }
        //      E
        if((E == '-' || E == 'S' || E == 'J' || E == '7') && !ex_E){
            graph[current].push_back(aoc::point(current.x+1,current.y));
        }
        //      S
        if((S == '|' || S == 'S' || S == 'J' || S == 'L') && !ex_S){
            graph[current].push_back(aoc::point(current.x,current.y+1));
        }
        //      W
        if((W == '-' || W == 'S' || W == 'F' || W == 'L') && !ex_W){
            graph[current].push_back(aoc::point(current.x-1,current.y));
        }
        // push any new neighbours onto stack

        for(const auto& p : graph[current])
        {
            if(graph.find(p) == graph.end()) {
                unvisited.push(p);
            }
        }
    }

    // dijkstra to end

    std::map<aoc::point,long> distances;
    std::map<aoc::point,aoc::point> previous;
    auto cmp = [&](const auto & left, const auto & right){ return distances[right] < distances[left];};
    std::priority_queue<aoc::point, std::vector<aoc::point>, decltype(cmp)> vertex_queue(cmp);

    for(const auto& v: graph)
    {
        distances[v.first] = INT32_MAX;
        if(Context.pipes[v.first] == 'S') distances[v.first] = 0;
        vertex_queue.push(v.first);
    }

    while(!vertex_queue.empty())
    {
        aoc::point current = vertex_queue.top();
        vertex_queue.pop();

        long min_distance = distances[current];

        for(const auto & neighbour : graph[current])
        {
            long alt = distances[current] + 1;
            if(alt < distances[neighbour]) {
                distances[neighbour] = alt;
                previous[neighbour] = current;
                vertex_queue.push(neighbour);
            }
        }
    }

    std::cout << std::max_element(
        std::begin(distances),
        std::end(distances),
        [](const auto & l, const auto & r) {
            return l.second < r.second;
        })->second << '\n';

    // part 2 

    int max_dim = 0, answer_2 = 0;
    for(const auto & p : Context.pipes)
    {
        max_dim = p.first.x > max_dim ? p.first.x : max_dim;
        max_dim = p.first.y > max_dim ? p.first.y : max_dim;
    }

    // less brain dead way of doing this:
    // scan each row 
    // note if we see a | or a F*J or a L*7
    // this is a crossing
    // odd number of crossings + vacant spot = add
    //
    // state table
    // 
    // 0: standard
    // 1: saw an F, stay in 1 for as long as we see -
    // 2: saw an L, stay in 2 as long as we see -
    for(int y = 0; y < max_dim; ++y){
        int crossings = 0, state = 0;
        for(int x = 0; x < max_dim; ++x){
            if(crossings % 2 == 1 && graph.find(aoc::point(x,y)) == graph.end())
                answer_2++;
            if(graph.find(aoc::point(x,y)) != graph.end()){
                char pipe = Context.pipes[aoc::point(x,y)];
                if(pipe == '|' || pipe == 'S') { // S is hard coded and not general
                    crossings++;
                    state = 0;
                } else if (pipe == 'F') {
                    state = 1;
                } else if (pipe == 'L') {
                    state = 2;
                } else if (state == 1 && pipe == 'J') {
                    crossings++;
                    state = 0;
                } else if (state == 1 && pipe == '7') {
                    state = 0;
                } else if (state == 2 && pipe == '7') {
                    crossings++;
                    state = 0;
                }else if (state == 2 && pipe == 'J') {
                    state = 0;
                }
            }

        }
    }

    std::cout << answer_2 << '\n';

    return 0;
}
