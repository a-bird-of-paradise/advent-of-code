#include "state_dumper.hpp"
#include "funcs.hpp"
#include <map>
#include <fstream>
#include <set>

auto dump_states(const std::vector<std::string>& lines) -> void
{

    int s_x, s_y;

    for(int y = 0; y < lines.size(); ++y) {
        for(int x = 0; x < lines[y].size(); ++x) {
            if(lines[y][x] == 'S') {
                s_x = x;
                s_y = y;
            }
        }
    }

    std::map<std::vector<std::string>,std::vector<int>> states_central
        = state_maker(lines,263);

    std::vector<std::string> from_left, from_right, from_top, from_bottom;
    from_left = lines;
    from_left[s_y][s_x] = '.';
    from_left[s_y][0] = 'S';

    from_right = lines;
    from_right[s_y][s_x] = '.';
    from_right[s_y][from_right[s_y].size()-1] = 'S';

    from_top = lines;
    from_top[s_y][s_x] = '.';
    from_top[0][s_x] = 'S';

    from_bottom = lines;
    from_bottom[s_y][s_x] = '.';
    from_bottom[from_bottom.size()-1][s_x] = 'S';

    std::map<std::vector<std::string>,std::vector<int>> states_left
        = state_maker(from_left,263);

    std::map<std::vector<std::string>,std::vector<int>> states_right
        = state_maker(from_right,263);

    std::map<std::vector<std::string>,std::vector<int>> states_top
        = state_maker(from_top,263);

    std::map<std::vector<std::string>,std::vector<int>> states_bottom
        = state_maker(from_bottom,263);

    std::map<std::vector<std::string>,std::vector<int>> states_top_left
        = state_maker_top_left(lines,263);

    std::map<std::vector<std::string>,std::vector<int>> states_top_right
        = state_maker_top_right(lines,263);

    std::map<std::vector<std::string>,std::vector<int>> states_bottom_right
        = state_maker_bottom_right(lines,263);

    std::map<std::vector<std::string>,std::vector<int>> states_bottom_left
        = state_maker_bottom_left(lines,263);
    
    std::ofstream states_left_out("out_left.txt");
    std::ofstream states_right_out("out_right.txt");
    std::ofstream states_top_out("out_top.txt");
    std::ofstream states_bottom_out("out_bottom.txt");
    std::ofstream states_bottom_left_out("out_bottom_left.txt");
    std::ofstream states_bottom_right_out("out_bottom_right.txt");
    std::ofstream states_top_left_out("out_top_left.txt");
    std::ofstream states_top_right_out("out_top_right.txt");
    std::ofstream states_central_out("out_central.txt");

    std::set<int> seen;

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_central)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_central_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_central_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_central_out << '\n';
                }
    }
    states_central_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_bottom_left)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_bottom_left_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_bottom_left_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_bottom_left_out << '\n';
                }
    }
    states_bottom_left_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_bottom_right)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_bottom_right_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_bottom_right_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_bottom_right_out << '\n';
                }
    }
    states_bottom_right_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_top_left)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_top_left_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_top_left_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_top_left_out << '\n';
                }
    }
    states_top_left_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_top_right)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_top_right_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_top_right_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_top_right_out << '\n';
                }
    }
    states_top_right_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_left)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_left_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_left_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_left_out << '\n';
                }
    }
    states_left_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_right)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_right_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_right_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_right_out << '\n';
                }
    }
    states_right_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_top)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_top_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_top_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_top_out << '\n';
                }
    }
    states_top_out.close();

    seen.clear();
    for(int i = 0; i < 263; i++)
    {
        if(seen.contains(i)) continue;
        for(const auto & p : states_bottom)
            for(const auto & v : p.second) 
                if(v == i) {
                    for(const auto & l : p.first) 
                        states_bottom_out << l << '\n';
                    for(const auto & z : p.second) {
                        states_bottom_out<< z << ' ';
                        seen.insert(z);
                    }
                    states_bottom_out << '\n';
                }
    }
    states_left_out.close();

    int64_t answer_1 = 0;

    for(const auto & p : states_central)
        for(const auto & v : p.second)
            if(v == 63)
                for(const auto & l : p.first) 
                    for(const auto & c : l) 
                        if(c == 'O') 
                            answer_1++;

    std::cout << answer_1 << '\n';
}
