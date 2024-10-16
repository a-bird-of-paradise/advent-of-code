use std::fs;
use std::env;

use intcode::do_intcode;
mod intcode;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let part1_numbers: Vec<i64> = numbers.clone();
    let input_1 : Vec<i64> = vec![1];
    let mut output_1 : Vec<i64> = Vec::new();

    do_intcode(part1_numbers, &input_1, &mut output_1);

    for i in output_1 { println!("{i}");}

    let part2_numbers = numbers.clone();
    let input_2 : Vec<i64> = vec![2];
    let mut output_2 : Vec<i64> = Vec::new();

    do_intcode(part2_numbers, &input_2, &mut output_2);

    for i in output_2 { println!("{i}");}

}
