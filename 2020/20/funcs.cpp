#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto operator<<(std::ostream& stream, const aoc::tile& tile) -> std::ostream&
{
    for(const auto & row : tile) std::cout << row << '\n';
    return stream;
}

auto operator<<(std::ostream& stream, const aoc::field_t& field) -> std::ostream&
{
    for(const auto & row : field) {
        for(const auto & col : row) {
            std::cout << '[' << col.t << ',' << col.r << "] ";
        }
        std::cout << '\n';
    }
    return stream;
}

auto rotate_identity(aoc::tile to_rotate) -> aoc::tile {return to_rotate;}

auto rotate_right(aoc::tile to_rotate) -> aoc::tile
{
    aoc::tile to_reverse;

    to_reverse.resize(std::size(to_rotate[0]));
    for(auto&& row : to_reverse) row.resize(to_rotate.size());

    for(std::size_t row = 0; row < to_rotate.size(); ++row) 
        for(std::size_t col = 0; col < to_rotate[row].size(); ++col) 
            to_reverse[col][row] = to_rotate[row][col];

    aoc::tile answer = to_reverse;
    for(std::size_t row = 0; row < to_reverse.size(); ++row)
        for(std::size_t col = 0; col < to_reverse[row].size(); ++col)
            answer[row][to_reverse[row].size() - col - 1] = to_reverse[row][col];

    return answer; 
}

auto rotate_left(aoc::tile to_rotate) -> aoc::tile
{
    aoc::tile to_reverse;

    to_reverse.resize(std::size(to_rotate[0]));
    for(auto&& row : to_reverse) row.resize(to_rotate.size());

    for(std::size_t row = 0; row < to_rotate.size(); ++row) 
        for(std::size_t col = 0; col < to_rotate[row].size(); ++col) 
            to_reverse[col][row] = to_rotate[row][col]; 

    aoc::tile answer = to_reverse;
    for(std::size_t row = 0; row < to_reverse.size(); ++row)
        for(std::size_t col = 0; col < to_reverse[row].size(); ++col)
            answer[to_reverse.size() - row - 1][col] = to_reverse[row][col];

    return answer; 
}

auto rotate_180(aoc::tile to_rotate) -> aoc::tile   {   return rotate_left(rotate_left(to_rotate)); }

auto flip_horizontal(aoc::tile to_rotate) -> aoc::tile 
{
    aoc::tile answer = to_rotate;
    for(std::size_t row = 0; row < to_rotate.size(); ++row)
        for(std::size_t col = 0; col < to_rotate[row].size(); ++col)
            answer[to_rotate.size() - row - 1][col] = to_rotate[row][col];

    return answer; 
}

auto flip_vertical(aoc::tile to_rotate) -> aoc::tile 
{
    aoc::tile answer = to_rotate;
    for(std::size_t row = 0; row < to_rotate.size(); ++row)
        for(std::size_t col = 0; col < to_rotate[row].size(); ++col)
            answer[row][to_rotate[row].size() - col - 1] = to_rotate[row][col];

    return answer; 
}

auto flip_diag(aoc::tile to_rotate) -> aoc::tile
{
    aoc::tile to_reverse;

    to_reverse.resize(std::size(to_rotate[0]));
    for(auto&& row : to_reverse) row.resize(to_rotate.size());

    for(std::size_t row = 0; row < to_rotate.size(); ++row) 
        for(std::size_t col = 0; col < to_rotate[row].size(); ++col) 
            to_reverse[col][row] = to_rotate[row][col]; 
    
    return to_reverse;
}

auto flip_antidiag(aoc::tile to_rotate) -> aoc::tile    {   return flip_diag(rotate_180(to_rotate));    }

auto apply_rotation(int id, aoc::tile to_rotate) -> aoc::tile
{
    assert(id >= 1 && id <= 8);
    switch(id)
    {
        case    1:  return rotate_identity(to_rotate);  break;
        case    2:  return rotate_left(to_rotate);      break;
        case    3:  return rotate_right(to_rotate);     break;
        case    4:  return rotate_180(to_rotate);       break;
        case    5:  return flip_vertical(to_rotate);    break;
        case    6:  return flip_horizontal(to_rotate);  break;
        case    7:  return flip_diag(to_rotate);        break;
        case    8:  return flip_antidiag(to_rotate);    break;
        default:    assert(false); return(to_rotate);
    }
}

auto could_x_be_left_of_y(const aoc::tile& x, const aoc::tile& y) -> bool
{
    bool answer = true;
    for(std::size_t i = 0; i < x.size(); ++i)
        if(x[i][x[i].size()-1] != y[i][0])
            answer = false;
    return answer; 
}

auto could_x_be_right_of_y(const aoc::tile& x, const aoc::tile& y) -> bool
{
    bool answer = true;
    for(std::size_t i = 0; i < x.size(); ++i)
        if(x[i][0] != y[i][y[i].size()-1])
            answer = false;
    return answer; 
}

auto could_x_be_up_of_y(const aoc::tile& x, const aoc::tile& y) -> bool
{
    return x[x.size()-1] == y[0];
}

auto could_x_be_down_of_y(const aoc::tile& x, const aoc::tile& y) -> bool
{
    return x[0] ==  y[y.size()-1];
}

auto could_x_be_left_of_y_state(const std::map<int,aoc::tile>& tiles, const aoc::tile_state& x, const aoc::tile_state& y) -> bool
{
    static std::map<std::pair<int64_t,int64_t>,bool> cache;

    auto seek = cache.find(std::make_pair(10000 * x.r + x.t, 10000*y.r+y.t));
    if(seek != cache.end()) return seek->second;

    aoc::tile left, right; 

    left = apply_rotation(x.r, tiles.at(x.t));
    right= apply_rotation(y.r, tiles.at(y.t));
    bool answer = could_x_be_left_of_y(left,right);
    cache[std::make_pair(10000 * x.r + x.t, 10000*y.r+y.t)] = answer;
    return answer;
}

auto could_x_be_down_of_y_state(const std::map<int,aoc::tile>& tiles, const aoc::tile_state& x, const aoc::tile_state& y) -> bool
{
    static std::map<std::pair<int64_t,int64_t>,bool> cache;

    auto seek = cache.find(std::make_pair(10000 * x.r + x.t, 10000*y.r+y.t));
    if(seek != cache.end()) return seek->second;

    aoc::tile left, right; 

    left = apply_rotation(x.r, tiles.at(x.t));
    right= apply_rotation(y.r, tiles.at(y.t));
    bool answer = could_x_be_down_of_y(left,right);
    cache[std::make_pair(10000 * x.r + x.t, 10000*y.r+y.t)] = answer;
    return answer;

}

auto is_empty_state(const aoc::tile_state& x) -> bool { return x.r == 0 && x.t == 0; }

auto backtrack_root(const std::map<int, aoc::tile>& tiles) -> aoc::field_t
{
    int square_size = static_cast<int>(std::sqrtf(tiles.size()));

    aoc::field_t field;
    field.resize(square_size);
    for(std::size_t i = 0; i < square_size; ++i) field[i].resize(square_size);
    return field;
}

auto backtrack_reject(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> bool
{
    bool answer = false;

    for(std::size_t i = 0; i < field.size(); ++i) {
        if(answer) break;
        for(std::size_t j = 0; j < field[i].size(); ++j) {
            if(answer) break; 

            // if empty, continue
            if(is_empty_state(field[i][j])) continue; 

            // check right neighbour if exists
            if(j < field[i].size() - 1 && !is_empty_state(field[i][j+1])) {
                if(!could_x_be_left_of_y_state(tiles,field[i][j], field[i][j+1])) {
                    answer = true;
                }
            }
            if(answer) break; 
            // check down neighbour if exists
            if(i < field.size() - 1 && !is_empty_state(field[i+1][j])) {
                if(!could_x_be_down_of_y_state(tiles,field[i][j],field[i+1][j])) {
                    answer = true;
                }
            }
        }
    }
    return answer; 
}
auto backtrack_accept(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> bool
{
    bool answer = true; 
    // field must be full 
    for(const auto & row : field)
        for(const auto & col : row)
            if(is_empty_state(col))
                answer = false; 

    if(answer) {
        for(std::size_t i = 0; i < field.size()-1; ++i) {
            if(!answer) break;
            for(std::size_t j = 0; j < field[i].size()-1; ++j) {
                if(!answer) break; 

                // check right neighbour if exists
                if(!could_x_be_left_of_y(
                    apply_rotation(field[i][j].r  ,tiles.at(field[i][j].t)  ),
                    apply_rotation(field[i][j+1].r,tiles.at(field[i][j+1].t))
                )) {
                    answer = false;
                }

                if(!answer) break; 

                // check down neighbour if exists
                if(!could_x_be_down_of_y(
                    apply_rotation(field[i][j].r  ,tiles.at(field[i][j].t)  ),
                    apply_rotation(field[i+1][j].r,tiles.at(field[i+1][j].t))
                )) {
                    answer = false;
                }
            }
        }
    }
    return answer; 
};

auto backtrack_first(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> std::pair<bool,aoc::field_t>
{
    bool found = true, breakout = false;
    aoc::field_t answer = field;

    std::vector<int> placed_tiles; 
    for(const auto & row : field)
        for(const auto & col : row)
            if(!is_empty_state(col))
                placed_tiles.push_back(col.t);
    
    // find first spare tile
    int tile_id = 0;
    for(const auto & tile : tiles) {
        if(std::find(std::begin(placed_tiles), std::end(placed_tiles), tile.first) == std::end(placed_tiles)) {
            tile_id = tile.first;
            break;
        }
    }
    if(tile_id == 0) found = false;

    // emplace
    if(found) {
        for(auto && row : answer) {
            if(breakout) break;
            for(auto && col : row) {
                if(col.t == 0) {
                    col.r = 1; 
                    col.t = tile_id;
                    breakout = true;
                    break;
                }
            }
        }
    }
    return std::make_pair(found,answer);
}
auto backtrack_next(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> std::pair<bool,aoc::field_t>
{
    bool found = false;
    int last_i, last_j; 
    // seek last placed tile
    for(std::size_t i = 0; i < field.size(); ++i) {
        if(found) break; 

        for(std::size_t j = 0; j < field[i].size(); ++j) {
            if(is_empty_state(field[i][j])) {
                if(j > 0) {
                    last_i = i;
                    last_j = j - 1;                    
                } else {
                    last_i = i-1; 
                    last_j = field[i].size()-1;                     
                }
                found = true;
                break;
            }
        }
    }

    aoc::field_t answer = field;

    if(!found) return std::make_pair(found,answer);

    // if already last rotation, replace with the next smallest unused int. else increment rotation.
    if(field[last_i][last_j].r == 8) {
        std::vector<int> placed_tiles; 
        for(const auto & row : field)
            for(const auto & col : row)
                if(!is_empty_state(col))
                    placed_tiles.push_back(col.t);
        
        // find first spare tile
        int tile_id = 0;
        int last_tile = field[last_i][last_j].t;
        for(const auto & tile : tiles) {
            if(tile.first > last_tile && std::find(std::begin(placed_tiles), std::end(placed_tiles), tile.first) == std::end(placed_tiles)) {
                tile_id = tile.first;
                break;
            }
        }
        if(tile_id == 0){
            found = false;
        } else {
            answer[last_i][last_j].t = tile_id;
            answer[last_i][last_j].r = 1;
        }
    } else {
        answer[last_i][last_j].r++; 
    }

    return std::make_pair(found,answer); 
}

auto backtrack(const std::map<int,aoc::tile>& tiles, aoc::field_t& answer) -> void
{
    static bool done = false;
    if(backtrack_reject(tiles,answer)) return; 
    if(backtrack_accept(tiles,answer)) done = true;
    if(done) return;

    std::pair<bool,aoc::field_t> state = backtrack_first(tiles,answer);

    while(state.first & !done) {
        backtrack(tiles, state.second);
        if(done) { answer = state.second; return; }
        state = backtrack_next(tiles, state.second); 
    }
    return;
}

auto render(const std::map<int, aoc::tile>& tiles, const aoc::field_t& field) -> aoc::tile
{
    std::size_t first_i = 1, last_i, first_j = 1, last_j;
    
    last_i = tiles.begin()->second.size()-1;
    last_j = tiles.begin()->second[0].size()-1; 

    aoc::tile answer;

    std::map<aoc::tile_state,aoc::tile> inners, outers;

    for(const auto & row : field) 
        for(const auto & col : row) 
            outers[col] = flip_horizontal(apply_rotation(col.r, tiles.at(col.t)));

    for(const auto & tile : outers) {
        aoc::tile to_insert;
        for(std::size_t i = 1; i < tile.second.size()-1; ++i) {
            to_insert.push_back(tile.second[i].substr(1,tile.second[i].size()-2));
        }
        inners[tile.first] = to_insert;
    }

    int total_i = (last_i - 1) * field.size(), n = last_i - 1;

    answer.clear(); 
    std::string line; 

    for(std::size_t i = 0; i < total_i; ++i) {
        for(std::size_t j = 0; j < total_i; ++j) {
            line.push_back(inners[field[i / n][j / n]][i % n][j % n]);
        }
        answer.push_back(line);
        line.clear();
    }

    return flip_horizontal(answer);

}

auto get_monster(void) -> std::vector<std::string> 
{
    std::vector<std::string> answer;
    answer.push_back("                  # ");
    answer.push_back("#    ##    ##    ###");
    answer.push_back(" #  #  #  #  #  #   ");
    return answer;
}

auto monster_could_be_at(const aoc::tile& tile, int row, int col) -> bool 
{
    static const std::vector<std::string> monster = get_monster();

    if(col + monster[0].size() > tile[0].size()) return false; 
    if(row + monster.size() > tile.size()) return false; 

    bool answer = true;

    for(std::size_t i = 0; i < monster.size(); ++i) {
        for(std::size_t j = 0; j < monster[i].size(); ++j) {
            if(monster[i][j] == ' ') continue;
            if(monster[i][j] == tile[row+i][col+j]) continue;
            answer = false;
        }
    }
    return answer; 
}