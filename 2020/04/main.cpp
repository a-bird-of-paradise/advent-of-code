#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    int64_t valid_1 = 0, valid_2 = 0;

    for(const auto& p : Context.passports)
    {
        if(p.size() == 8 || (p.size() == 7 && !p.contains("cid"))) {

            valid_1++;
            
            // part 2: type checking
            if(
                p.at("byr").t == aoc::data_type::four_int
                && p.at("iyr").t == aoc::data_type::four_int
                && p.at("eyr").t == aoc::data_type::four_int
                && (p.at("hgt").t == aoc::data_type::in || p.at("hgt").t == aoc::data_type::cm)
                && p.at("hcl").t == aoc::data_type::hex
                && p.at("ecl").t == aoc::data_type::string
                && p.at("pid").t == aoc::data_type::nine_int
            )   {

                // part 2: range checking

                if(
                    p.at("byr").four_int_val >= 1920 && p.at("byr").four_int_val <= 2002
                    && p.at("iyr").four_int_val >= 2010 && p.at("iyr").four_int_val <= 2020
                    && p.at("eyr").four_int_val >= 2020 && p.at("eyr").four_int_val <= 2030
                    && (
                        (p.at("hgt").t == aoc::data_type::in && (
                            p.at("hgt").in_val >= 59 && p.at("hgt").in_val <= 76
                        ))
                        || (p.at("hgt").t == aoc::data_type::cm && (
                            p.at("hgt").cm_val >= 150 && p.at("hgt").cm_val <= 193
                        ))
                    )
                    && (
                        p.at("ecl").string_val == "amb"
                        || p.at("ecl").string_val == "blu"
                        || p.at("ecl").string_val == "brn"
                        || p.at("ecl").string_val == "gry"
                        || p.at("ecl").string_val == "grn"
                        || p.at("ecl").string_val == "hzl"
                        || p.at("ecl").string_val == "oth"
                    )
                )   {   valid_2++;  }
            }
        }
    }

    std::cout << valid_1 << '\n' << valid_2 << '\n';

    return 0;
}

// not 75