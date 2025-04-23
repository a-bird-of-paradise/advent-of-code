use std::{env,fs};
mod part_1;
mod part_2;
mod parser;

use part_1::do_part_1;
use part_2::{do_part_1_again,compose_actions};
use parser::read_lines;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");

    println!("Part 1:         {}",do_part_1(&raw_file));
    println!("Part 1 (again): {}",do_part_1_again(&raw_file));

    let action_list = read_lines(&raw_file).unwrap().1;
    // merci: https://codeforces.com/blog/entry/72593
    let big_composed_action = compose_actions(&action_list, 119315717514047);
    let big_power_action = big_composed_action.power(101741582076661);

    let inv = mod_mul_inverse(big_power_action.a, 119315717514047);
    let ans =  ( ( (2020 - big_power_action.b) as i128 ) * (inv as i128) ).rem_euclid(119315717514047 as i128);
    println!("part 2:         {ans}");
}

fn pow_mod(x: i64, n: i64, m: i64) -> i64 {
    let mut answer : i64 = 1;
    let mut n_ = n;
    let mut x_ = x;
    while n_ > 0 {
        if n_ % 2 != 0 {
            answer = ( (answer as i128 * x_ as i128).rem_euclid(m as i128) ) as i64;
        }
        n_ /= 2;
        x_ = ((x_ as i128 * x_ as i128).rem_euclid(m as i128)) as i64;
    }
    answer
}

fn mod_mul_inverse(x: i64, m: i64) -> i64 {
    pow_mod(x, m-2, m)
}