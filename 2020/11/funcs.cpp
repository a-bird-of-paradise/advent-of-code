#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto print(const std::vector<std::string>& board) -> void
{
    for(const auto& s : board)  std::cout << s << '\n';
    std::cout << '\n';
}

auto tick(const std::vector<std::string>& board) -> std::vector<std::string>
{
    std::vector<std::string> answer = board;
    
    std::size_t imin, imax, jmin, jmax;

    for(std::size_t i = 0; i < board.size(); ++i)
    {
        if(i == 0) imin = 0; else imin = i-1;
        if(i == board.size()-1) imax = i; else imax = i+1;

        for(std::size_t j = 0; j < board[i].size(); ++j) 
        {
            if(j == 0) jmin = 0; else jmin = j-1;
            if(j == board[i].size()-1) jmax = j; else jmax = j+1;

            if(board[i][j] == 'L')
            {
                bool all_empty = true;
                
                for(std::size_t k = imin; k <= imax; ++k)
                {
                    for(std::size_t l = jmin; l <= jmax; ++l)
                    {
                        if(board[k][l] == '#') 
                        {
                            all_empty = false;
                        }
                    }
                }

                if(all_empty) answer[i][j] = '#';
            }
            else if(board[i][j] == '#')
            {
                int counter = 0;
                
                for(std::size_t k = imin; k <= imax; ++k)
                {
                    for(std::size_t l = jmin; l <= jmax; ++l)
                    {
                        if(board[k][l] == '#') 
                        {
                            counter++;
                        }
                    }
                }

                if(counter > 4) answer[i][j] = 'L'; // will always be at least 1 due to self so need strict here

            }
        }
    }
    return answer;
}

auto equals(const std::vector<std::string>& left, const std::vector<std::string>& right) -> bool
{
    if(left.size() != right.size()) 
    {
        return false;
    }

    for(std::size_t i = 0; i < left.size(); ++i)
    {
        if(left[i].size() != right[i].size()) 
        {   
            return false;
        }

        for(std::size_t j = 0; j < left[i].size(); ++j)
        {
            if(left[i][j] != right[i][j])
            {
                return false;
            }
        }
    }

    return true;

}

auto get_index(
    const std::size_t x,
    const std::size_t y, 
    const std::size_t n_y) -> std::size_t
{
    return (x * n_y) + y;
}

auto index_to_x(
    const std::size_t index,
    const std::size_t n_y) -> std::size_t
{
    return index / n_y; // int division
}

auto index_to_y(
    const std::size_t index, 
    const std::size_t n_y) -> std::size_t
{
    return index % n_y; 
}

auto build_neighbours(
    const std::vector<std::string>& board
) -> std::map<std::size_t, std::vector<std::size_t>>
{

    std::map<std::size_t, std::vector<std::size_t>> answer;
    
    std::size_t imin, imax, jmin, jmax, n_y;

    n_y = board[0].size();

    for(std::size_t i = 0; i < board.size(); ++i) {

        if(i == 0) imin = 0; else imin = i-1;
        if(i == board.size()-1) imax = i; else imax = i+1;

        for(std::size_t j = 0; j < board[i].size(); ++j) {

            if(j == 0) jmin = 0; else jmin = j-1;
            if(j == board[i].size()-1) jmax = j; else jmax = j+1;

            if(board[i][j] != 'L') continue;

            for(std::size_t k = imin; k <= imax; ++k) {
                for(std::size_t l = jmin; l <= jmax; ++l) {
                    if(k == i && l == j) continue;

                    if(board[k][l] == 'L') {
                        answer[get_index(i,j,n_y)].push_back(get_index(k,l,n_y));
                    }
                }
            }
        }
    }

    return answer;

}

auto tick_state(
    const std::string& state,
    const std::map<std::size_t, std::vector<std::size_t>>& neighbours,
    const std::size_t n_y,
    const uint16_t n_occupied 
) -> std::string
{
    std::string answer = state;

    for(const auto & cell : neighbours)
    {
        if(state[cell.first] == 'L') {

            bool no_occupied_neighbours = true;

            for(const auto & n : cell.second) {
                if(state[n] == '#') {
                    no_occupied_neighbours = false;
                    break;
                }
            }

            if(no_occupied_neighbours) {
                answer[cell.first] = '#';
            }

        } else if (state[cell.first] == '#') {

            int n_occupied_neighbours = 0;

            for(const auto & n : cell.second) {
                if(state[n] == '#') {
                    n_occupied_neighbours++;
                }
            }

            if(n_occupied_neighbours >= n_occupied) {
                answer[cell.first] = 'L';
            }
        }
    }

    return answer;

}

auto build_neighbours_2(
    const std::vector<std::string>& board
) -> std::map<std::size_t, std::vector<std::size_t>>
{

    std::map<std::size_t, std::vector<std::size_t>> answer;
    
    std::size_t n_y;

    n_y = board[0].size();

    for(std::size_t i = 0; i < board.size(); ++i) {
        for(std::size_t j = 0; j < board[i].size(); ++j) {
            // N
            int64_t k = i-1;
            while(k >= 0) {
                if(board[k][j] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(k,j,n_y));
                    break;
                }
                k--;
            }

            // S
            k = i+1;
            while(k < board.size()) {
                if(board[k][j] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(k,j,n_y));
                    break;
                }
                k++;
            }
            // E
            k = j+1;
            while(k < board[i].size()) {
                if(board[i][k] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(i,k,n_y));
                    break;
                }
                k++;
            }
            // W
            k = j-1;
            while(k >= 0) {
                if(board[i][k] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(i,k,n_y));
                    break;
                }
                k--;
            }
            // SE
            k = i + 1;
            int64_t l = j + 1;
            while(k < board.size() && l < board[k].size()) {
                if(board[k][l] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(k,l,n_y));
                    break;
                }
                k++;
                l++;
            }
            // NE
            k = i - 1;
            l = j + 1;
            while(k >= 0 && l < board[k].size()) {
                if(board[k][l] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(k,l,n_y));
                    break;
                }
                k--;
                l++;
            }
            // SE
            k = i + 1;
            l = j - 1;
            while(k < board.size() && l >= 0) {
                if(board[k][l] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(k,l,n_y));
                    break;
                }
                k++;
                l--;
            }
            // SW
            k = i - 1;
            l = j - 1;
            while(k >= 0 && l >= 0) {
                if(board[k][l] == 'L') {
                    answer[get_index(i,j,n_y)].push_back(get_index(k,l,n_y));
                    break;
                }
                k--;
                l--;
            }
        }
    }

    return answer;

}