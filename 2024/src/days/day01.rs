use aoc2024::AOC;
use num::abs;
use std::collections::BTreeMap;
pub struct Day01 {}

use nom::{
    character::complete::{i64, line_ending, space1}, sequence::{terminated,separated_pair}, IResult, Parser, multi::many1, combinator::opt
};

fn i64_ws_i64(input: &str) -> IResult<&str,(i64,i64)> {
    Ok(separated_pair(i64, space1, i64).parse(input)?)
}

fn line(input: &str) -> IResult<&str,(i64,i64)> {
    Ok(terminated(i64_ws_i64, opt(line_ending)).parse(input)?)
}

fn read_lines(input: &str) -> IResult<&str,Vec<(i64,i64)>> {
    many1(line).parse(input)
}

impl AOC for Day01 {
    fn part_one(&self, input: &str) -> String {
        let parsed = read_lines(input).unwrap().1;
        let (mut left, mut right): (Vec<_>, Vec<_>) = parsed.into_iter().map(|(a, b)| (a, b)).unzip();
        left.sort();
        right.sort();

        let dist_sum : i64 = left.iter().zip(right.iter())
            .map(|(l,r)| abs(l-r))
            .sum();

        dist_sum.to_string()
    }

    fn part_two(&self, input: &str) -> String {
        let parsed = read_lines(input).unwrap().1;
        let (left, right): (Vec<_>, Vec<_>) = parsed.into_iter().map(|(a, b)| (a, b)).unzip();

        let mut right_map = BTreeMap::<i64,usize>::new();

        for i in 0..right.len() {
            *right_map.entry(right[i]).or_default() += 1;
        }

        for (k,v) in &mut right_map {
            *v *= *k as usize;
        }

        let answer: usize = left
            .into_iter()
            .map(|x| *right_map.entry(x).or_default())
            .sum();

        answer.to_string()

    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;    

    #[test]
    fn parser() {
        let test_file_path = "test-inputs/day01.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let parsed = read_lines(&file);
        assert_eq!(parsed,Ok(("", vec![(3, 4), (4, 3), (2, 5), (1, 3), (3, 9), (3, 3)])))
    }

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day01.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");

        let my_struct = Day01 {};
        assert_eq!(my_struct.part_one(&file),"11");
    }
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day01.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");

        let my_struct = Day01 {};
        assert_eq!(my_struct.part_two(&file),"31");
    }
}