#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    // part 1: accumulate

    std::unique_ptr<aoc::BranchNode> accum, current, temp;

    for(auto & i : Context.SnailNumbers)
    {
        if(accum.get() == nullptr){
            accum = i->clone();
        } else {
            temp = std::move(accum);
            current = i->clone();
            accum = std::make_unique<aoc::BranchNode>(std::move(temp),std::move(current));
            aoc::reduce_tree(accum.get());
        }
    }

    std::cout << accum->magnitude() << std::endl;

    // part 2: largest pair

    long largest = 0, this_pair_value;
    std::unique_ptr<aoc::BranchNode> first, second;

    for(const auto & i : Context.SnailNumbers) {
        for(const auto & j : Context.SnailNumbers) {
            if(i.get() == j.get()) continue; // different!!!!!!
            first = i->clone();
            second = j->clone();
            temp = std::make_unique<aoc::BranchNode>(std::move(first),std::move(second));
            aoc::reduce_tree(temp.get());
            this_pair_value = temp->magnitude();
            largest = largest < this_pair_value ? this_pair_value : largest;
        }
    }

    std::cout << largest << std::endl;

    return 0;

}