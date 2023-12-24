#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <map>

bool noisy;

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    noisy = false;

    int64_t answer_1_test = 0, answer_1_real = 0;

    for(auto i = 0; i < Context.lines.size(); ++i)
    {
        for(auto j = 0; j < i; ++j) {
            auto p = intersect_2d(Context.lines[i],Context.lines[j]);

            // test

            if(inside_2d(p,7,27)) { // cross in region
                if( 
                    (p.first.first > Context.lines[i][0] && Context.lines[i][3] > 0)                            // line 1 crosses ahead
                    || (p.first.first < Context.lines[i][0] && Context.lines[i][3] < 0) 
                ) {
                    if(
                        (p.first.first > Context.lines[j][0] && Context.lines[j][3] > 0)                        // line 2 crosses ahead
                        || (p.first.first < Context.lines[j][0] && Context.lines[j][3] < 0)
                    ) {
                        answer_1_test++;
                    }
                }
            }

            // real

            if(inside_2d(p,200000000000000LL,400000000000000LL)) { // cross in region
                if( 
                    (p.first.first > Context.lines[i][0] && Context.lines[i][3] > 0)                            // line 1 crosses ahead
                    || (p.first.first < Context.lines[i][0] && Context.lines[i][3] < 0) 
                ) {
                    if(
                        (p.first.first > Context.lines[j][0] && Context.lines[j][3] > 0)                        // line 2 crosses ahead
                        || (p.first.first < Context.lines[j][0] && Context.lines[j][3] < 0)
                    ) {
                        answer_1_real++;
                    }
                }
            }
        }
    }

    std::cout << answer_1_test << "\n" << answer_1_real << '\n';

    // part 2 
    // nudge velocities so the rock is stationary... 

    std::map<int,bool> permitted_x, permitted_y, permitted_z;

    std::vector<aoc::bigint_t> hit_times;

    int n_i = 5;
    if(Context.lines.size() > 100) n_i = 500;

    for(int i = -n_i; i < n_i; ++i) {
        permitted_x[i] = true;
        permitted_y[i] = true;
        permitted_z[i] = true;
    }

    for(int i = 0; i < Context.lines.size(); ++i) {
        for(int j =0 ; j < Context.lines.size(); ++ j) {
            if(i==j) continue;
            // if x0 > x1 && vx0 > vx1 then (vx0,vx1) is impossible
            if(Context.lines[i][0] > Context.lines[j][0] && Context.lines[i][3] > Context.lines[j][3]) {
                for(int k = Context.lines[j][3]+1; k < Context.lines[i][3]-1; ++k) {
                    permitted_x[k]=false;
                }
            }
            if(Context.lines[i][1] > Context.lines[j][1] && Context.lines[i][4] > Context.lines[j][4]) {
                for(int k = Context.lines[j][4]+1; k < Context.lines[i][4]-1; ++k) {
                    permitted_y[k]=false;
                }
            }
            if(Context.lines[i][2] > Context.lines[j][2] && Context.lines[i][5] > Context.lines[j][5]) {
                for(int k = Context.lines[j][5]+1; k < Context.lines[i][5]-1; ++k) {
                    permitted_z[k]=false;
                }
            }

        }
    }

    bool found_hits = false;
    int64_t found_dx, found_dy, found_dz, found_x, found_y, found_z;

    for(const auto & xp : permitted_x) {
        if(!xp.second) continue;
        for(const auto & yp : permitted_y) {
            if(!yp.second) continue;
            auto dx = xp.first, dy = yp.first, dz = 0;

            auto nudged = Context.lines;

            for(auto&& n : nudged) {
                n[3] -= dx; 
                n[4] -= dy; 
                n[5] -= dz;
            }

            bool done_one = false, impossible = false; 
            aoc::point_t common_collide, zero;

            for(int i = 0; i < nudged.size(); i++) 
            {
                for(int j = 0; j < i; ++j) {
                    std::pair<aoc::point_t,bool> hit = intersect_2d(nudged[i],nudged[j]);

                    if(hit.second == false) { impossible = true; break; }

                    if(!done_one) {
                        done_one = true;
                        common_collide = hit.first;
                        continue;
                    }

                    if(common_collide != hit.first && hit.first != zero) { // sentinel for coincidental
                        impossible = true;
                        break;
                    }
                }
                noisy = false;
                if(impossible) break;
            }
            if(impossible) continue;

            common_collide = intersect_2d(nudged[0],nudged[1]).first;

            for(int i = 0; i < nudged.size(); ++i){
                hit_times.push_back((common_collide.first.numerator() - nudged[i][0])/nudged[i][3]);
                assert(common_collide.first.denominator() == 1);
            }

            found_hits = true;
            found_dx = dx;
            found_dy = dy; 
            found_x = nudged[0][0] + static_cast<int64_t>(hit_times[0]) * (nudged[0][3]) ;
            found_y = nudged[0][1] + static_cast<int64_t>(hit_times[0]) * (nudged[0][4]) ;
            
            break;
        }
        if(found_hits) break;
    }

    // now which z

    for(const auto zp : permitted_z)
    {
        if(zp.second == false) continue;
        auto nudged = Context.lines;

        for(auto&& n : nudged) {
            n[3] -= found_dx; 
            n[4] -= found_dy; 
            n[5] -= zp.first;
        }

        auto z_0 = nudged[0][2]+static_cast<int64_t>(hit_times[0]*nudged[0][5]), 
            z_1 = nudged[1][2]+static_cast<int64_t>(hit_times[1]*nudged[1][5]);

        if(z_0 == z_1) {
            found_dz = zp.first;
            found_z = z_0;
            break;
        } 
    }

    std::cout << found_x+found_y+found_z << '\n';

    return 0;
}