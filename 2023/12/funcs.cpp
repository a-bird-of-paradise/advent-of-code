#include "funcs.hpp"
#include "scanner.hpp"
#include <algorithm>
#include "context.hpp"
#include <regex>
#include <unordered_map>

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto generate_candidates(const std::vector<char>& current,
                        std::size_t level,
                        std::set<std::vector<char>>& candidates,
                        const std::vector<int64_t>& report) -> void
{

    //for(const auto& c : current) std::cout << c; std::cout <<'\n';

    if(level >= current.size()) {
        candidates.insert(current);
        return; 
    }
    if(current[level] != '?') {
        generate_candidates(current,level+1,candidates,report);
    } else {

        std::vector<char> first = current, second = current;

        first[level] = '.';
        second[level] = '#';

        if(!is_definitely_invalid(first,report)) generate_candidates(first,level+1,candidates,report);
        if(!is_definitely_invalid(second,report)) generate_candidates(second,level+1,candidates,report);
    }

}

auto get_report(const std::vector<char>& candidate) -> const std::vector<int64_t>
{
    enum class state {INITIAL, DOT, HASH};

    std::vector<int64_t> seen;

    state current_state = state::INITIAL;
    auto it = candidate.begin();
    int64_t current_length = 0;

    while(it != candidate.end())
    {
        char current = *it;

        if(current == '?') break;

        switch(current_state)
        {
            case state::INITIAL:
                if(current == '#') {
                    current_state = state::HASH; 
                    current_length++;
                } else {
                    current_state = state::DOT;
                }
                break;
            
            case state::HASH:
                if(current == '#') {
                    current_length++;
                } else {
                    current_state = state::DOT;
                    seen.push_back(current_length);
                    current_length = 0;
                }
                break;

            case state::DOT:
                if(current == '#') {
                    current_length = 1;
                    current_state = state::HASH;
                } else {
                    // do nothing
                }
                break;
        }
        ++it;
    }

    if(current_state == state::HASH) seen.push_back(current_length);

    return seen;
}

auto is_valid_candidate(const std::vector<char>& candidate, 
                        const std::vector<int64_t>& report) -> bool
{
    bool answer = get_report(candidate) == report;

    if(false) {
        for(const auto& c: candidate) std::cout << c; std::cout << " | ";
        for(const auto& r: report) std::cout << r << " "; std::cout << '\n';
    }

    return answer;
}

auto is_definitely_invalid(
    const std::vector<char>& candidate, 
    const std::vector<int64_t>& report) -> bool
{
    std::vector<int64_t> x = get_report(candidate);

    // permit the last of x to be smaller, but that's it

    if(x.size() > report.size()) return true;
    if(x.size() == 0) return false;

    bool answer = false;

    for(std::size_t i = 0; i < x.size() - 1; i++)
    {
        if(x[i] != 
        report[i]) 
        {
            answer = true;
            break;
        }
    }

    if(!answer && x.back() > report[x.size()-1]) answer = true;

    return answer;
}


auto recurse(const std::string& candidate, const std::vector<int64_t>& unused) -> int64_t
{
    static std::unordered_map<std::string,int64_t> cache;

    // operates on "tails"
    // base case - no more springs

    if(unused.size() == 0) {
        if(candidate.contains('#') == false) {
            return 1;
        } else {
            return 0;
        }
    }
    // unused.size >= 1... ran out of room?
    if(candidate.size() == 0) {
        return 0;
    }
    // no more space?
    if(candidate.contains('#') == false && candidate.contains('?') == false) {
        return 0;
    }

    std::string key = candidate;
    for(const auto & u : unused) key += "_" + std::to_string(u);

    auto lookup = cache.find(key);

    if(lookup != cache.end()) return lookup->second;

    // ok, find first possible match
    // want to find string of #? of length n, that is not followed by a #
    // so slurp up initital dots...

    std::string candidate_pattern = "^\\.*[#\\?]{";
    candidate_pattern += std::to_string(unused[0]);
    candidate_pattern += "}(?!#)"; // and negative lookahead
    std::regex candidate_regex(candidate_pattern);
    std::smatch candidate_results;

    std::string leading_hashes_pattern = "^\\.*#+";
    std::regex hashes_regex(leading_hashes_pattern);
    std::smatch hashes_results;

    std::string leading_question_pattern = "^\\.*\\?";
    std::regex question_regex(leading_question_pattern);
    std::smatch question_results;

    bool matched = std::regex_search(candidate,candidate_results,candidate_regex);
    bool matched_hash = std::regex_search(candidate,hashes_results,hashes_regex);
    bool matched_q = std::regex_search(candidate,question_results,question_regex);

    // if not matched
    // if matched_hash then fail
    // if matched_q then consume and retry

    if(!matched) {
        if(matched_hash) {
            return 0;
        } else if(matched_q) {
            return recurse(candidate.substr(question_results[0].str().size()),unused);
        } else {
            assert(false);
        }
    } else /* matched */ {
        int64_t use_result = 0, unused_result = 0;

        std::string x;

        if(candidate_results[0].str().size() < candidate.size()){
            x = candidate.substr(candidate_results[0].str().size()+1);
        }

        auto y_begin = std::next(std::begin(unused));
        auto y_end = std::end(unused);

        std::vector<int64_t> y(y_begin,y_end);
        use_result = recurse(x,y);

        if(matched_q) {
            unused_result = recurse(candidate.substr(question_results[0].str().size()), unused);
        } else /* matched_hash */ {
            // do nothing
        }

        cache[key] = use_result + unused_result;

        return use_result + unused_result;
    }
}