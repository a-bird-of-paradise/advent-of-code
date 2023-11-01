#pragma once
#include "location.hh"

namespace aoc
{

    class context
    {
    public:
        context() 
            : 
            xmin(0), xmax(0), ymin(0), ymax(0)
            { 
                loc.initialize(); 
            }

        location loc;
        int xmin, xmax, ymin, ymax, min_dx, min_dy, max_dx, max_dy;

    };
}