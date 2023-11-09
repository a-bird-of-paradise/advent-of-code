#include "funcs.hpp"
#include "scanner.hpp"
#include <queue>
#include <set>
#include <cmath>

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

std::ostream& operator<< (std::ostream& stream, const aoc::state& s)
{
    switch(s) {
        case aoc::state::vacant:
            stream << '.';
            break;
        case aoc::state::unoccupiable:
            stream << '_';
            break;
        case aoc::state::amber:
            stream << 'A';
            break;
        case aoc::state::bronze:
            stream << 'B';
            break;
        case aoc::state::copper:
            stream << 'C';
            break;
        case aoc::state::desert:
            stream << 'D';
            break;
    }
    return stream;
}

std::ostream& operator<< (std::ostream& stream, const aoc::board& b)
{
    for(std::size_t i = 0; i < 11; i++) stream << b.hallway[i];
    stream << ' ' << b.cost << '\n' << ' ';
    for(std::size_t i = 0; i < 4; i++) {
        for(std::size_t j = 0; j < 4; j++) {
            stream << ' ' << b.pockets[j][i];
        }
        stream << '\n' << ' ';
    }
    stream << '\n';
    return stream;
}

unsigned long item_cost(const aoc::state& item)
{
    unsigned long answer = 0;

    switch(item)
    {
        case    aoc::state::amber:
            answer = 1;
            break;
        case    aoc::state::bronze:
            answer = 10;
            break;
        case    aoc::state::copper:
            answer = 100;
            break;
        case    aoc::state::desert:
            answer = 1000;
            break;
        default:
            assert(false); // never happens
    }

    return answer;
}

aoc::board::board() : cost(0)
{
    for(std::size_t i = 0; i < 11; i++  ) hallway[i] = aoc::state::vacant;
    for(std::size_t i = 2; i < 9; i += 2) hallway[i] = aoc::state::unoccupiable;
    for(std::size_t i = 0; i < 4; i++)
        for(std::size_t j = 0; j < 4; j++)
            pockets[i][j] = aoc::state::vacant;
}

aoc::state from_letter(char letter)
{
    switch(letter)
    {
        case    'A':
            return aoc::state::amber;
        case    'B':
            return aoc::state::bronze;
        case    'C':
            return aoc::state::copper;
        case    'D':
            return aoc::state::desert;
        default:
            assert(false); //never happens
    }
}


std::size_t which_pocket(aoc::state x)
{
    if(x == aoc::state::amber) return 0;
    if(x == aoc::state::bronze) return 1;
    if(x == aoc::state::copper) return 2;
    if(x == aoc::state::desert) return 3;
    assert(false);
    return 3; // desert
}

bool is_amber(const aoc::state& x) { return x == aoc::state::amber; }
bool is_bronze(const aoc::state& x) { return x == aoc::state::bronze; }
bool is_copper(const aoc::state& x) { return x == aoc::state::copper; }
bool is_desert(const aoc::state& x) { return x == aoc::state::desert; }
bool is_vacant(const aoc::state& x) { return x == aoc::state::vacant; }
bool is_unoccupiable(const aoc::state& x) { return x == aoc::state::unoccupiable; }
bool is_item(const aoc::state& x) { return is_amber(x) || is_bronze(x) || is_copper(x) || is_desert(x); }
bool is_not_item(const aoc::state& x) { return !is_item(x); }

unsigned long solve(const aoc::board& game)
{

    std::priority_queue<aoc::board, std::vector<aoc::board>, aoc::board_priority> neighbours;
    std::set<aoc::board> visited;

    neighbours.push(game);

    aoc::board current, current_costless;

    bool show = false;
    unsigned long ticker = 0;

    while(!neighbours.empty())
    {
        if(show) show = false;
        ticker += 1;

        //std::cout << visited.size() << " | " << neighbours.size() << ' ' << ticker << std::endl;

        current = neighbours.top();
        neighbours.pop();
        current_costless = current;
        current_costless.cost = 0;
        if(visited.contains(current_costless)) continue;
        visited.insert(current_costless);

        if(show) std::cout << current;

        // terminal state?

        bool terminal = true;

        for(std::size_t j = 0; j < 4; j++){
            if( ! ( is_amber(current.pockets[0][j]) || is_unoccupiable(current.pockets[0][j]))) terminal = false;
            if( ! ( is_bronze(current.pockets[1][j]) || is_unoccupiable(current.pockets[1][j]))) terminal = false;
            if( ! ( is_copper(current.pockets[2][j]) || is_unoccupiable(current.pockets[2][j]))) terminal = false;
            if( ! ( is_desert(current.pockets[3][j]) || is_unoccupiable(current.pockets[3][j]))) terminal = false;
        }

        if(terminal) return current.cost;
        
        // enqueue possible moves. 

        // first up: from the hallway into a pocket 

        for(std::size_t i = 0; i < 11; i++) 
        {
            aoc::state current_item = current.hallway[i];

            // nothing to move
            if(is_not_item(current_item))
                continue;

            std::size_t desired_pocket_index = which_pocket(current_item);

            // is the 'target' pocket available?
            unsigned long empty_slots = 0;
            bool other_occupant = false;
            for(std::size_t j = 0; j < 4; j++) {
                if(is_vacant(current.pockets[desired_pocket_index][j])) {
                    empty_slots++;
                } else if (is_unoccupiable(current.pockets[desired_pocket_index][j])) {
                    continue;
                } else if (current.pockets[desired_pocket_index][j] != current_item) {
                    other_occupant = true;
                }
            }

            if(empty_slots == 0 || other_occupant) 
                continue;
            // can I move from left to right into the pocket?   
            // i.e. we know the pocket is clear but can I get above it? 
            std::size_t target_index = (2* (1+desired_pocket_index));

            bool path_free = true;

            if(i < target_index){
                for(std::size_t j = i + 1; j < target_index; j++){
                    if(is_item(current.hallway[j])) {
                        path_free = false;
                    }
                }
            } else { // i > target_index
                for(std::size_t j = i - 1; j > target_index; j--) {
                    if(is_item(current.hallway[j])) {
                        path_free = false;
                    }
                }
            }

            if(!path_free) 
                continue;

            // ok path is clear. 
            // which pocket index do I want? 

            std::size_t pocket_index = 999; // fault on invalid access

            for(std::size_t j = 0; j < 4; j++) { // 0 must be free
                if(current.pockets[desired_pocket_index][j] == aoc::state::vacant) {
                    pocket_index = j;
                }
            }

            assert(pocket_index >= 0 && pocket_index < 4);

            // phew. path length is:

            std::size_t path_length = std::max(i,target_index) - std::min(i,target_index) + 1 + pocket_index;
            unsigned long new_cost = path_length * item_cost(current_item);

            aoc::board new_board = current; // copy

            // do move
            new_board.pockets[desired_pocket_index][pocket_index] = current_item;
            new_board.hallway[i] = aoc::state::vacant;
            new_board.cost += new_cost;

            if(show) std::cout << new_board;

            // add to queue

            neighbours.push(new_board);
        }

        // second up: from the top of a pocket into a hallway

        for(std::size_t i = 0; i < 4; i++) {

            aoc::state top_of_pocket = aoc::state::vacant;
            std::size_t top_of_pocket_index = 0;

            for(std::size_t j = 0; j<4; j++) {
                if(is_item(current.pockets[i][j])){
                    top_of_pocket = current.pockets[i][j];
                    top_of_pocket_index = j;
                    break;
                }
            }

            // skip empty pocket 
            if(is_not_item(top_of_pocket)) continue;

            // what is above the pocket
            std::size_t start_index = (2* (1+i));

            // go left

            for(std::size_t j = start_index - 1; j >= 0; j--) {
                aoc::state corridor_contents = current.hallway[j];
                if(corridor_contents == aoc::state::unoccupiable){
                    continue;
                } else if (corridor_contents == aoc::state::vacant) {
                    // can move

                    std::size_t path_length = std::max(j,start_index) - std::min(j,start_index) + 1 + top_of_pocket_index;
                    unsigned long new_cost = path_length * item_cost(top_of_pocket);

                    aoc::board new_board(current); // copy

                    // do move
                    new_board.hallway[j] = top_of_pocket;
                    new_board.pockets[i][top_of_pocket_index] = aoc::state::vacant;
                    new_board.cost += new_cost;

                    if(show) std::cout << new_board;

                    // add to queue
                    neighbours.push(new_board);
                } else { // blocked
                    break;
                }
                if(j == 0) break;
            }

            // go right

            for(std::size_t j = start_index + 1; j < 11; j++) {
                aoc::state corridor_contents = current.hallway[j];
                if(corridor_contents == aoc::state::unoccupiable){
                    continue;
                } else if (corridor_contents == aoc::state::vacant) {
                    // can move

                    std::size_t path_length = std::max(j,start_index) - std::min(j,start_index) + 1 + top_of_pocket_index;
                    unsigned long new_cost = path_length * item_cost(top_of_pocket);

                    aoc::board new_board(current); // copy

                    // do move
                    new_board.hallway[j] = top_of_pocket;
                    new_board.pockets[i][top_of_pocket_index] = aoc::state::vacant;
                    new_board.cost += new_cost;


                    if(show) std::cout << new_board;

                    // add to queue
                    neighbours.push(new_board);
                } else { // blocked
                    break;
                }
            }
        }
    }
    return 0;
}

// 12521