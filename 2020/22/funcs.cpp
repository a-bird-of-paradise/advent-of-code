#include "funcs.hpp"
#include "scanner.hpp"
#include <set>

auto combat(std::deque<int> p1, std::deque<int> p2, bool noisy) -> uint64_t
{
    uint64_t round = 0;

    while(p1.size() > 0 && p2.size() > 0)
    {
        int p1_plays = p1.front();
        int p2_plays = p2.front();

        if(noisy) {
            std::cout << "-- Round " << round+1 << " --\n";
            std::cout << "Player 1's deck: ";
            for(const auto & card : p1) std::cout << card << ", ";
            std::cout << "\n";
            std::cout << "Player 2's deck: ";
            for(const auto & card : p2) std::cout << card << ", ";
            std::cout << "\n";
            std::cout << "Player 1 plays: " << p1_plays << "\n";
            std::cout << "Player 2 plays: " << p2_plays << "\n";
        }
        
        p1.pop_front();
        p2.pop_front();

        if(p1_plays > p2_plays)
        {
            p1.push_back(p1_plays);
            p1.push_back(p2_plays);
            if(noisy) std::cout << "Player 1 wins the round! \n\n";
        } else {
            p2.push_back(p2_plays);
            p2.push_back(p1_plays);
            if(noisy) std::cout << "Player 2 wins the round! \n\n";
        }

        ++round;
    }

    if(noisy) {
        std::cout << "== Post-game results ==\n";
        std::cout << "Player 1's deck: ";
        for(const auto & card : p1) std::cout << card << ",";
        std::cout << "\n";
        std::cout << "Player 2's deck: ";
        for(const auto & card : p2) std::cout << card << ",";
        std::cout << "\n";
    }

    std::deque<int> winner;
    winner = p1.size() == 0 ? p2 : p1;

    std::size_t index = 0, index_max = winner.size(), score_counter = winner.size(); 

    uint64_t score = 0; 

    for(index = 0; index < index_max; ++index, --score_counter) {
        score += score_counter * winner[index];
    }

    return score;
}

auto recursive_combat(std::deque<int> p1, std::deque<int> p2, bool noisy) -> std::pair<uint64_t,int> // score, winner 1 or 2
{
    uint64_t round = 0;
    int win_id = 0;

    std::set<std::pair<std::deque<int>,std::deque<int>>> seen; 

    while(p1.size() > 0 && p2.size() > 0)
    {

        if(seen.find(std::make_pair(p1,p2)) != seen.end()) {
            win_id = 1;
            break;
        }

        seen.insert(std::make_pair(p1,p2)); 

        int p1_plays = p1.front();
        int p2_plays = p2.front();

        if(noisy) {
            std::cout << "-- Round " << round+1 << " --\n";
            std::cout << "Player 1's deck: ";
            for(const auto & card : p1) std::cout << card << ", ";
            std::cout << "\n";
            std::cout << "Player 2's deck: ";
            for(const auto & card : p2) std::cout << card << ", ";
            std::cout << "\n";
            std::cout << "Player 1 plays: " << p1_plays << "\n";
            std::cout << "Player 2 plays: " << p2_plays << "\n";
        }
        
        p1.pop_front();
        p2.pop_front();

        if(p1_plays <= p1.size() && p2_plays <= p2.size()) {
            std::deque<int> p1_pass, p2_pass;
            for(int i = 0; i < p1_plays; ++i) p1_pass.push_back(p1[i]);
            for(int i = 0; i < p2_plays; ++i) p2_pass.push_back(p2[i]);
            std::pair<uint64_t,int> result = recursive_combat(std::move(p1_pass), std::move(p2_pass),noisy);
            if(result.second == 1) {
                p1.push_back(p1_plays);
                p1.push_back(p2_plays);
            } else if (result.second == 2) {
                p2.push_back(p2_plays);
                p2.push_back(p1_plays);
            } else {
                assert(false);
            }
            
        } else if(p1_plays > p2_plays)  {
            p1.push_back(p1_plays);
            p1.push_back(p2_plays);
            if(noisy) std::cout << "Player 1 wins the round! \n\n";
        } else {
            p2.push_back(p2_plays);
            p2.push_back(p1_plays);
            if(noisy) std::cout << "Player 2 wins the round! \n\n";
        }

        ++round;
    }

    if(noisy) {
        std::cout << "== Post-game results ==\n";
        std::cout << "Player 1's deck: ";
        for(const auto & card : p1) std::cout << card << ",";
        std::cout << "\n";
        std::cout << "Player 2's deck: ";
        for(const auto & card : p2) std::cout << card << ",";
        std::cout << "\n";
    }

    std::deque<int> winner;
    if(win_id == 0) win_id = p1.size() > p2.size() ? 1 : 2;
    winner = win_id == 1 ? p1 : p2;

    std::size_t index = 0, index_max = winner.size(), score_counter = winner.size(); 

    uint64_t score = 0; 

    for(index = 0; index < index_max; ++index, --score_counter) {
        score += score_counter * winner[index];
    }

    return std::make_pair(score,win_id);

}