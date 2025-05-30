use aoc2024::AOC;
use aoc2024::skip_nth;
pub struct Day02 {}

use nom::{
    character::complete::{i64, line_ending, space1}, 
    sequence::{terminated}, 
    IResult, 
    Parser, 
    multi::{many1,separated_list1},
    combinator::opt
};

fn i64_ws_sep_list(input: &str) -> IResult<&str,Vec<i64>> {
    separated_list1(space1, i64).parse(input)
}

fn line(input: &str) -> IResult<&str,Vec<i64>> {
    Ok(terminated(i64_ws_sep_list, opt(line_ending)).parse(input)?)
}

fn read_lines(input: &str) -> IResult<&str,Vec<Vec<i64>>>{
    many1(line).parse(input)
}

fn try_drops(row: &[i64]) -> bool {
    if safe_row(row) {
        return true;
    }

    for n in 0..row.len() {
        if safe_row(&skip_nth(row, n)) {
            return true;
        }
    }
    false
}

fn safe_row(row: &[i64]) -> bool {
    let mut first_col = true;
    let mut any_decreasing = false;
    let mut any_increasing = false; 
    let mut any_stable = false; 
    let mut any_wild = false; 
    let mut current = row[0];
    let mut prev: i64;

    for col in row {
        if first_col {
            first_col = false;
            continue
        }
        prev = current;
        current = *col;

        match current - prev {
            1..=std::i64::MAX  => any_increasing = true,
            0                  => any_stable = true,
            std::i64::MIN..=-1 => any_decreasing = true
        }

        match current - prev {
            -3..=3 => (),
            _ => any_wild = true
        }

    }

    (any_decreasing != any_increasing) && !any_wild && !any_stable

}

impl AOC for Day02 {
    fn part_one(&self, input: &str) -> String {
        let parsed = read_lines(input).unwrap().1;
        let mut safe_count = 0;

        for row in parsed {
            if safe_row(&row) {
                safe_count+=1;
            }
        }
        safe_count.to_string()
    }

    fn part_two(&self, input: &str) -> String {        
        let parsed = read_lines(input).unwrap().1;
        let mut safe_count = 0;

        for row in parsed {
            if try_drops(&row) {
                safe_count+=1;
            }
        }
        safe_count.to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn parser() {
        let test_file_path = "test-inputs/day02.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let parsed = read_lines(&file);
        assert_eq!(
            parsed,
            Ok(("", 
                vec![
                    vec![7, 6, 4, 2, 1], 
                    vec![1, 2, 7, 8, 9], 
                    vec![9, 7, 6, 2, 1], 
                    vec![1, 3, 2, 4, 5], 
                    vec![8, 6, 4, 4, 1], 
                    vec![1, 3, 6, 7, 9]
                ]
            ))
        );
    }

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day02.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");

        let my_struct = Day02 {};
        assert_eq!(my_struct.part_one(&file),"2");
    }

    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day02.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");

        let my_struct = Day02 {};
        assert_eq!(my_struct.part_two(&file),"4");
    }

    #[test]
    fn test_skip_nth() {
        let orig = vec![0,1,2,3];
        assert_eq!(skip_nth(&orig,1),vec![0,2,3]);
    }

    #[test]
    fn test_safe_row() {
        let never_safe = vec![1,2,7,8,9];
        let always_safe = vec![7,6,4,2,1];
        let eventually_safe = vec![1,3,2,4,5];
        assert_eq!(safe_row(&always_safe),true);
        assert_eq!(safe_row(&never_safe),false);
        assert_eq!(safe_row(&eventually_safe),false);
    }

    #[test]
    fn test_drops() {
        let never_safe = vec![1,2,7,8,9];
        let always_safe = vec![7,6,4,2,1];
        let eventually_safe = vec![1,3,2,4,5];
        assert_eq!(try_drops(&always_safe),true);
        assert_eq!(try_drops(&never_safe),false);
        assert_eq!(try_drops(&eventually_safe),true);
    }
}