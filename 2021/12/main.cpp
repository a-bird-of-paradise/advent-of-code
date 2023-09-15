#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>

void dfs(const aoc::graph_t& graph, 
    std::vector<std::string>& current_path,
    const std::set<std::string>& small_nodes,
    std::set<std::string>& visited_small_nodes,
    std::string start_node,
    std::vector< std::vector<std::string>>& paths,
    bool visited_any_small_node_twice)
{
    current_path.push_back(start_node);

    if(start_node == "end") {
        paths.push_back(current_path);
        current_path.pop_back();
        return;
    }

    for(auto & child: graph.at(start_node)) {
        if(small_nodes.contains(child)){
            if(visited_small_nodes.contains(child)){
                if(!visited_any_small_node_twice && child != "start" && child != "end") {
                    dfs(graph,current_path,small_nodes,visited_small_nodes,child,paths,true);
                }
            } else {
                visited_small_nodes.insert(child);
                dfs(graph,current_path,small_nodes,visited_small_nodes,child,paths,visited_any_small_node_twice);
                visited_small_nodes.erase(child);
            }
        } 
        else {
            dfs(graph,current_path,small_nodes,visited_small_nodes,child,paths,visited_any_small_node_twice);
        }
    }
    current_path.pop_back();
}

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

/*     
    for(auto i : Context.graph) {
        std::cout << i.first << " [ ";
        for(auto j : i.second) std::cout << j << " ";
        std::cout << "]" << std::endl;
    }
 */
    std::vector<std::string> path;
    std::vector<std::vector<std::string>> paths;
    std::set<std::string> visited_small_nodes;
    visited_small_nodes.insert("start");

    dfs(Context.graph,
        path,
        Context.small_nodes,
        visited_small_nodes,
        "start",
        paths,
        true);
/*
    for(auto & i : paths) {
        for(auto & j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
*/

    std::cout << paths.size() << std::endl;

    // part 2

    path.clear();
    paths.clear();
    visited_small_nodes.clear(); 
    visited_small_nodes.insert("start");

    dfs(Context.graph,
        path,
        Context.small_nodes,
        visited_small_nodes,
        "start",
        paths,
        false);
/*
    for(auto & i : paths) {
        for(auto & j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
*/
    std::cout << paths.size() << std::endl;

    return 0;
}