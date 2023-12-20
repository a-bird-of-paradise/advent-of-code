#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto print(const std::map<std::string,aoc::module>& modules) -> void
{
    for(const auto & mod : modules)
    {
        std::cout << "Name: " << mod.first << ", Type: " << (int)mod.second.type << ", ";
        std::cout << "Destinations: ";
        for(const auto & dest : mod.second.destinations) std::cout << dest << ", ";
        if(mod.second.type == aoc::node_type::FLIPFLOP) std::cout << "State: " << mod.second.flip_flop_state << ", ";
        if(mod.second.type == aoc::node_type::CONJUNCTION) {
            std::cout << "Memory: ";
            for(const auto & name : mod.second.conjunction_memory) 
                std::cout << name.first << ":" << name.second << ", ";
        }
        std::cout << "Output: " << mod.second.output_high;
        std::cout << '\n';
    }
}

auto process_queue(
    std::queue<aoc::pulse>& input, 
    std::queue<aoc::pulse>& output, 
    std::map<std::string,aoc::module>& modules,
    std::map<bool,int64_t>& counter) -> bool
{
    bool any_low = false, answer_2 = false;
    while(!output.empty()) output.pop();
    while(!input.empty())
    {
        aoc::pulse current = std::move(input.front());
        input.pop();

        switch(modules[current.to].type)
        {
            case aoc::node_type::BROADCASTER:
                for(const auto & name : modules[current.to].destinations)
                    {
                    output.push({current.to,name,current.is_high});
                    counter[current.is_high]++;
                    }
                break;
            
            case aoc::node_type::FLIPFLOP:
                if(current.is_high == false) {
                    modules[current.to].flip_flop_state = !modules[current.to].flip_flop_state;
                    for(const auto & name : modules[current.to].destinations)
                    {
                        output.push({current.to,name,modules[current.to].flip_flop_state});
                        counter[modules[current.to].flip_flop_state]++;
                    }
                }
                break;

            case aoc::node_type::CONJUNCTION:

                modules[current.to].conjunction_memory[current.from] = current.is_high;
                any_low = false;
                for(const auto & state : modules[current.to].conjunction_memory) {
                    if(state.second == false) {
                        any_low = true;
                    }
                }

                for(const auto & name : modules[current.to].destinations)
                {
                    output.push({current.to,name,any_low});
                    counter[any_low]++;
                }

                if(current.to == "kl" && current.is_high) { std::cout << current.from << ' '; answer_2 = true; }

                break;

            case aoc::node_type::BUTTON:
                break;

            case aoc::node_type::SPECIAL:
                break;
        }
    }
    return answer_2;
}
    