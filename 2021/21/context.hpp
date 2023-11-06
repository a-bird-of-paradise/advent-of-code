#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    class context
    {
    public:
        context() : start_1(0), start_2(0)  { loc.initialize(); }
        location loc;
        int start_1, start_2;
    };

    class Die
    {
    public:
        Die() : roll_counter(0) {}
        virtual int roll() = 0;
        virtual unsigned long long roll_count() { return roll_counter; };
    protected:
        unsigned long long roll_counter;
    };

    class DeterministicDie : public Die
    {
    public:
        DeterministicDie() : last(0) , Die() {}
        virtual int roll() override {
            last++;
            roll_counter++;
            if(last>=101) last = 1;
            return last;
        }
    private:
        int last;
    };

}
