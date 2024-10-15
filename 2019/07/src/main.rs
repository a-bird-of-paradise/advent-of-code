use std::fs;
use std::env;
mod intcode;
use itertools::Itertools;
use crate::intcode::day7_part1_inner;
use crate::intcode::day7_part2_inner;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let part1_numbers: Vec<i64> = numbers.clone();
    let items_1 : Vec<i64> = vec![0,1,2,3,4];
    let perms_1 = items_1.iter().permutations(items_1.len());

    let mut answer_1 = i64::MIN;

    for perm_1 in perms_1 {
        let result : i64 = day7_part1_inner(&part1_numbers, &perm_1);
        answer_1 = if result > answer_1 { result } else { answer_1 };
    }
    println!("{answer_1}");

    let part2_numbers : Vec<i64> = numbers.clone(); 
    let items_2 : Vec<i64> = vec![5,6,7,8,9];
    let perms_2 = items_2.iter().permutations(items_2.len());

    let mut answer_2 = i64::MIN;

    for perm_2 in perms_2 {
        let result : i64 = day7_part2_inner(&part2_numbers, &perm_2);
        answer_2 = if result > answer_2 { result } else { answer_2 };
    }
    println!("{answer_2}");

}
