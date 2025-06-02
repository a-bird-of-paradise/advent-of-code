use aoc2024::AOC;

pub struct Day07 {} 

use nom::{
    bytes::complete::tag, 
    character::complete::{line_ending, space1, u64}, 
    combinator::opt, 
    multi::{many1, separated_list1}, 
    sequence::{separated_pair, terminated}, 
    IResult, 
    Parser
};

fn read_rhs(input: &str) -> IResult<&str,Vec<u64>> {
    separated_list1(space1, u64).parse(input)
}

fn read_line_inner(input: &str) -> IResult<&str, (u64, Vec<u64>)> {
    separated_pair(u64, tag(": "), read_rhs).parse(input)
}

fn read_line(input: &str) -> IResult<&str,(u64,Vec<u64>)> {
    terminated(read_line_inner, opt(line_ending)).parse(input)
}

fn read_file(input: &str) -> IResult<&str, Vec<(u64,Vec<u64>)>> {
    many1(read_line).parse(input)
}

fn reducible(target: u64, chain: &[u64], is_part_two: bool) -> bool {

    if chain.len() == 0 {
        return false;
    }

    if target == chain[0] && chain.len() == 1 {
        return true;
    }

    // option 1: divide by last in chain 

    if target % chain.last().unwrap() == 0 { 
        if reducible(target / chain.last().unwrap(), &chain[0..chain.len()-1], is_part_two) {
            return true;
        }
    }

    // option 2: subtract last in chain 

    if target > *chain.last().unwrap() {
        if reducible(target - chain.last().unwrap(), &chain[0..chain.len()-1], is_part_two) {
            return true;
        }
    }

    // option 3: concat 

    if is_part_two {

        // let [var] = num digits in base 10 representation of var 
        // target = x || y 
        // then target % (10 ^ [y]) == y  
        // and [target] > [x]
        
        let last_num_digits = chain.last().unwrap().ilog10()+1; 
        let left = (10 as u64).pow(last_num_digits); 
        let target_num_digits = target.ilog10()+1; 
        let tail = target % left; 

        if target_num_digits > last_num_digits && *chain.last().unwrap() == tail {
            if reducible(target / left, &chain[0..chain.len()-1], is_part_two) {
                return true;
            }
        }
    }

    false

}

impl AOC for Day07 {
    fn part_one(&self, input: &str) -> String {   

        read_file(input)
            .unwrap()
            .1
            .into_iter()
            .map(|(target,chain)| {
                if reducible(target, &chain, false) {
                    target 
                } else {
                    0
                }
            })
            .sum::<u64>()
            .to_string()
    }

    fn part_two(&self, input: &str) -> String {

        read_file(input)
            .unwrap()
            .1
            .into_iter()
            .map(|(target,chain)| {
                if reducible(target, &chain, true) { 
                    target 
                } else {
                    0
                }
            })
            .sum::<u64>()
            .to_string()
    }
}


#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day07.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day07 {};
        assert_eq!(my_struct.part_one(&file),"3749")
    }
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day07.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day07 {};
        assert_eq!(my_struct.part_two(&file),"11387");
    }
}
