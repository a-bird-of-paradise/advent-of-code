#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>

void board_print(const aoc::board_t& board)
{
    for(auto& i : board) {
        for(auto& j : i) std::cout << std::setw(4) << j;
        std::cout << std::endl;
    }
}

std::pair<long,long> board_play(const aoc::board_t& board, const std::vector<long>& draws)
{
    long plays = 0, score = 0, sub_score = 0;
    bool won = false;
    
    aoc::board_t played = board;
    for(std::size_t i = 0; i < played.size(); i++)
        for(std::size_t j = 0; j < played[i].size(); j++)
            played[i][j] = -1;

    while(!won && plays < draws.size())
    {
        for(std::size_t i = 0; i < board.size(); i++)
            for(std::size_t j = 0; j < board[i].size(); j++)
                if(board[i][j] == draws[plays])
                {
                    played[i][j] = draws[plays];
                }
        
        plays++;

        // test row
        for(std::size_t i = 0; i < board.size(); i++)   {
            won = true;
            for(std::size_t j = 0; j < board[i].size(); j++) {
                if(played[i][j] == -1) {
                    won = false;
                }
            }
            if(won) break;
         }
         // and cols if necessary
         if(!won)
         {
            for(std::size_t i = 0; i < board.size(); i++)   {
                won = true;
                for(std::size_t j = 0; j < board[i].size(); j++) {
                    if(played[j][i] == -1) {
                        won = false;
                    }
                }
                if(won) break;
            }
         }
    }
    // scoring
    for(std::size_t i = 0; i < board.size(); i++)
        for(std::size_t j = 0; j < board[i].size(); j++)
        {
            if(played[i][j]==-1) played[i][j]=0;
            score += board[i][j];
            sub_score += played[i][j];
        }

    score = (score - sub_score) * draws[plays-1];

    if(!won) score = -1;

    return std::make_pair(plays,score);
}

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    std::vector<std::pair<long,long>> scores;
    std::pair<long,long> result, result2;

    for(auto &i: Context.boards)
    {
        scores.push_back(board_play(i, Context.random_numbers));
    }

    // when do we win? 

    result = *std::min_element(
        scores.begin(),
        scores.end(),
        [](const auto& lhs, const auto& rhs){return lhs.first < rhs.first; }
    );

    result2 = *std::max_element(
        scores.begin(),
        scores.end(),
        [](const auto& lhs, const auto& rhs){return lhs.first < rhs.first; }
    );

    std::cout << result.second << std::endl << result2.second << std::endl;
    
    return 0;
}