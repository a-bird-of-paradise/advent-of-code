#include "funcs.hpp"
#include <ranges>
#include <set>

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

long pathfinder(const aoc::graph& Graph)
{

    std::cout << "pathfinding..." << std::endl;

    std::vector<long> dist(Graph.nodes.size(),INT32_MAX);
    std::vector<int> path(Graph.nodes.size(),-1);
    std::set<int> unvisited;

    for(auto & node : Graph.nodes) unvisited.insert(node.id);

    dist[0]=0;

    while(unvisited.size() != 0) {

        std::cout << unvisited.size() << std::endl;

        int min_id = -1;
        long min_distance = INT32_MAX;

        for(auto & i : unvisited) {
            if(dist[i] < min_distance) {
                min_distance = dist[i];
                min_id = i;
            }
        }

        if(min_id == Graph.nodes.back().id) break;

        unvisited.erase(min_id);

        auto edges = Graph.edges | std::views::filter(
            [&](const auto & e) {
                return e.source_id == min_id && unvisited.contains(e.destination_id);
            }
        );

        for(auto & e : edges)
        {
            long alt = dist[min_id] + e.weight;
            if(alt < dist[e.destination_id]) {
                dist[e.destination_id] = alt;
                path[e.destination_id] = min_id;
            }
        }
    }

    return dist.back();
}

aoc::graph make_graph(const std::vector<std::vector<int>>& board)
{

    std::cout << "graphing..." << std::endl;
    int node_counter = 0;

    aoc::graph Graph;

    for(std::size_t i = 0; i < board.size(); i++) {
        for(std::size_t j = 0; j < board[i].size(); j++) {
            aoc::node current {};
            current.x = i; current.y = j; current.id = node_counter++;
            Graph.nodes.push_back(current);
        }
    }

    for(std::size_t i = 0; i < board.size(); i++) {
        for(std::size_t j = 0; j < board[i].size(); j++) {
            std::cout << i << " " << j << std::endl;
            aoc::node current {}, next {};
            current = *std::find_if(
                Graph.nodes.begin(), 
                Graph.nodes.end(),
                [i,j](const auto & n) { return n.x == i && n.y == j; }
            );

            if(auto result = std::find_if(
                Graph.nodes.begin(), 
                Graph.nodes.end(),
                [i,j](const auto & n) { return n.x == i && n.y == j+1; });
                result != Graph.nodes.end()) {
                    aoc::edge the_edge;
                    the_edge.source_id = current.id;
                    the_edge.destination_id = result->id;
                    the_edge.weight = board[result->x][result->y];
                    Graph.edges.push_back(the_edge);
            }

            if(auto result = std::find_if(
                Graph.nodes.begin(), 
                Graph.nodes.end(),
                [i,j](const auto & n) { return n.x == i && n.y == j-1; });
                result != Graph.nodes.end()) {
                    aoc::edge the_edge;
                    the_edge.source_id = current.id;
                    the_edge.destination_id = result->id;
                    the_edge.weight = board[result->x][result->y];
                    Graph.edges.push_back(the_edge);
            }

            if(auto result = std::find_if(
                Graph.nodes.begin(), 
                Graph.nodes.end(),
                [i,j](const auto & n) { return n.x == i+1 && n.y == j; });
                result != Graph.nodes.end()) {
                    aoc::edge the_edge;
                    the_edge.source_id = current.id;
                    the_edge.destination_id = result->id;
                    the_edge.weight = board[result->x][result->y];
                    Graph.edges.push_back(the_edge);
            }

            if(auto result = std::find_if(
                Graph.nodes.begin(), 
                Graph.nodes.end(),
                [i,j](const auto & n) { return n.x == i-1 && n.y == j; });
                result != Graph.nodes.end()) {
                    aoc::edge the_edge;
                    the_edge.source_id = current.id;
                    the_edge.destination_id = result->id;
                    the_edge.weight = board[result->x][result->y];
                    Graph.edges.push_back(the_edge);
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