#include "funcs.hpp"
#include "scanner.hpp"
#include <algorithm>
#include "context.hpp"
#include <map>
#include <algorithm>

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto string_to_hand_type(std::string const& s) -> aoc::hand_type
{
    int count[14] = {0};

    for(const auto& c : s)
    {
        if(c >= '1' && c <= '9') {
            count[c - '1']++;
        } else if (c == 'T') {
            count[9]++;
        } else if (c == 'J') {
            count[10]++;
        } else if (c == 'Q') {
            count[11]++;
        } else if (c == 'K') {
            count[12]++;
        } else if (c == 'A') {
            count[13]++;
        }
    }

    int five,four,three,two,one;
    five = four = three = two = one = 0;

    for(const auto& i : count){
        if(i == 1) one ++;
        else if (i == 2) two ++;
        else if (i == 3) three ++;
        else if (i == 4) four ++;
        else if (i == 5) five++;
    }

    if(five == 1) return aoc::hand_type::five_of_a_kind;
    if(four == 1) return aoc::hand_type::four_of_a_kind;
    if(two == 1 && one == 3) return aoc::hand_type::one_pair;
    if(two == 2 && one == 1) return aoc::hand_type::two_pair;
    if(three == 1 && two == 1) return aoc::hand_type::full_house;
    if(three == 1 && one == 2) return aoc::hand_type::three_of_a_kind;
    return aoc::hand_type::high_card;

    assert(false);
}

auto card_to_number(char card) -> int16_t
{
    if(card >= '1' && card <= '9') return card - '0';
    if(card == 'T') return 10;
    if(card == 'J') return 11;
    if(card == 'Q') return 12;
    if(card == 'K') return 13;
    if(card == 'A') return 14;

    assert(false);
}

auto compare_cards(char l, char r) -> bool 
{
    return card_to_number(l) < card_to_number(r);
}

auto card_to_number_2(char card) -> int16_t
{
    if(card >= '1' && card <= '9') return card - '0';
    if(card == 'T') return 10;
    if(card == 'J') return 0;
    if(card == 'Q') return 12;
    if(card == 'K') return 13;
    if(card == 'A') return 14;

    assert(false);
}

auto compare_cards_2(char l, char r) -> bool 
{
    return card_to_number_2(l) < card_to_number_2(r);
}

const char cards[] = {'1','2','3','4','5','6','7','8','9','T','J','Q','K','A'};

auto string_to_hand_type_2(std::string const& s) -> aoc::hand_type
{
    if(std::find(s.begin(),s.end(),'J') == s.end()) return string_to_hand_type(s);

    std::string no_j = s;

    no_j.erase(std::remove(no_j.begin(),no_j.end(),'J'),no_j.end());

    char seen[14] = {0};

    for(auto const& x : no_j) { seen[card_to_number(x)-1]++; }

    int max_seen_count = 0;
    char max_seen = '1';

    for(auto const& x : cards) 
        if(seen[card_to_number(x)-1] > max_seen_count) {
            max_seen_count = seen[card_to_number(x)-1];
            max_seen = x;
        }

    std::string augmented = no_j;
    for(std::size_t i = no_j.size(); i < 5; i++) augmented.push_back(max_seen);

    return string_to_hand_type(augmented);

}