#include "funcs.hpp"
#include <map>
#include <queue> 
void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

long pathfinder(const aoc::graph& Graph)
{

    std::cout << "pathfinding..." << std::endl;

    std::vector<long> dist(Graph.nodes.size(),INT16_MAX); // avoid overflows
    std::vector<long> path(Graph.nodes.size(),-1);

    auto cmp = [&](const auto & left, const auto & right){ return dist[right.id] < dist[left.id];};
    std::priority_queue<aoc::node,std::vector<aoc::node>, decltype(cmp)> pq(cmp);
    for(auto i : Graph.nodes) pq.push(i);

    std::map<int, std::vector<aoc::edge>> edge_map;
    for(auto & i : Graph.edges) edge_map[i.source_id].push_back(i);

    dist[0]=0;

    while(pq.size() != 0) {

        int min_id = pq.top().id;
        pq.pop();

        int min_distance = dist[min_id];

        for(auto & e : edge_map[min_id])
        {
            //if(!pq.contains(e.destination_id)) continue;
            long alt = dist[min_id] + e.weight;
            if(alt < dist[e.destination_id]) {
                dist[e.destination_id] = alt;
                path[e.destination_id] = min_id;
                pq.push(Graph.nodes[e.destination_id]);
                // this creates duplicates in the PQ, but it refreshes the priority
                // so the last step will be popping lots of stale things from the pq
                // alternative is searching for them and updating and reordering PQ
                // hard to say which is more efficient 
            }
        }
    }

    return dist.back();
}

aoc::graph make_graph(const std::vector<std::vector<int>>& board)
{

    std::cout << "graphing..." << std::endl;
    long node_counter = 0;

    aoc::graph Graph;

    for(std::size_t i = 0; i < board.size(); i++) {
        for(std::size_t j = 0; j < board[i].size(); j++) {
            aoc::node current {};
            current.x = i; current.y = j; current.id = node_counter++;
            Graph.nodes.push_back(current);
        }
    }

    // assumption: square board
    // so (x,y) -> nx + y id 

    std::size_t n = board.size(); 

    for(std::size_t x = 0; x < n; x++){
        for(std::size_t y = 0; y < n; y++) {
            std::size_t id = n * x + y ; 
            aoc::node current = Graph.nodes[id];
            aoc::node next {};
            aoc::edge e {};
            // up 

            if(x>0) {
                next = Graph.nodes[n * (x-1) + y];
                e.source_id = id;
                e.destination_id = n * (x-1) + y;
                e.weight = board[x-1][y];
                Graph.edges.push_back(e);
            }

            // down

            if(x<n-1) {
                next = Graph.nodes[n * (x+1) + y];
                e.source_id = id;
                e.destination_id = n * (x+1) + y;
                e.weight = board[x+1][y];
                Graph.edges.push_back(e);
            }

            // left 

            if(y>0) {
                next = Graph.nodes[n * x + y-1];
                e.source_id = id;
                e.destination_id = n * x + y-1;
                e.weight = board[x][y-1];
                Graph.edges.push_back(e);
            }

            // right 

            if(y<n-1) {
                next = Graph.nodes[n * x + y + 1];
                e.source_id = id;
                e.destination_id = n * x + y + 1;
                e.weight = board[x][y+1];
                Graph.edges.push_back(e);
            }
        }
    }
    return Graph;
}

std::vector<std::vector<int>> embiggen(const std::vector<std::vector<int>>& tile)
{
    std::cout << "embiggenning..." << std::endl; 
    typedef std::vector<std::vector<int>> tile_t;

    std::vector<std::vector<tile_t>> tiles;
    tiles.resize(5);
    for(auto & i : tiles) i.resize(5);

    for(std::size_t i = 0; i < 5; i++){
        for(std::size_t j = 0; j < 5; j++){
            tiles[i][j]=tile;
        }
    }
            
    for(std::size_t i = 0; i < 5; i++) {
        for(std::size_t j = 0; j < 5; j++) {
            for(std::size_t k = 0; k < tiles[i][j].size(); k++){
                for(std::size_t l = 0; l < tiles[i][j][k].size(); l++) {
                    tiles[i][j][k][l] += i + j;
                    if(tiles[i][j][k][l] > 9){
                        tiles[i][j][k][l] %= 9;
                    }
                }
            }
        }
    }

    std::vector<int> row;
    tile_t answer;

    for(std::size_t i = 0; i < 5; i++) {
        for(std::size_t j = 0; j < tile.size(); j++) {
            row.clear();
            for(std::size_t k = 0; k < 5; k++) {
                for(std::size_t l = 0; l < tile[j].size(); l++) {
                    row.push_back(tiles[i][k][j][l]);
                }
            }
            answer.push_back(row);
        }
    }
    return(answer);
}