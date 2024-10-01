#include "funcs.hpp"
#include "scanner.hpp"

auto tick(std::vector<int32_t>& cups, const int32_t current_cup) -> int32_t
{
    int32_t n = cups.size();

    int32_t current_index = std::distance(
        std::begin(cups),
        std::find(
            std::begin(cups),
            std::end(cups),
            current_cup
        )
    );

    int32_t first,second,third;
    first = cups[(current_index + 1) % n];
    second = cups[(current_index + 2) % n];
    third = cups[(current_index + 3) % n];

    if(current_index + 4 <= n)
    {
    //    std::cout << 'a' << '\n';
        cups.erase(
            std::next(std::begin(cups),current_index + 1),
            std::next(std::begin(cups),current_index + 3 + 1)
        );
    }
    else if (current_index + 3 <= n)
    {
    //    std::cout << 'b' << '\n';
        cups.erase(
            std::next(std::begin(cups),current_index + 1),
            std::end(cups)
        );
        cups.erase(
            std::begin(cups),
            std::next(std::begin(cups),1)
        );
    }
    else if (current_index + 2 <= n)
    {
    //    std::cout << 'c' << '\n';
        cups.erase(
            std::next(std::begin(cups),current_index + 1),
            std::end(cups)
        );
        cups.erase(
            std::begin(cups),
            std::next(std::begin(cups),2)
        );
    }
   else if (current_index + 1 <= n)
    {
        //std::cout << 'd' << '\n';
        cups.erase(
            std::begin(cups),
            std::next(std::begin(cups),3)
        );
    }

    int32_t destination = current_cup-1;
    if(destination <= 0) destination = n;

    while(destination == first || destination == second || destination == third) {
        destination--;
        if(destination <= 0) destination = n;
    }

    //std::cout << current_cup << ' ' << destination << '\n';

    std::reference_wrapper<decltype(first)> myints[] = {first, second, third};

    std::vector<int32_t>::iterator it = std::find(std::begin(cups), std::end(cups), destination);

    if(it == std::end(cups)) {
        std::cout << first << ' ' << second << ' ' << third << ' ' << destination << '!' << std::endl;
        assert(false);
    }

    it = std::next(it);
    it = cups.insert(it,std::begin(myints),std::end(myints));

    it = std::find(std::begin(cups),std::end(cups),current_cup);
    it = std::next(it);
    if(it == std::end(cups)) it = std::begin(cups);
    return *it;

}