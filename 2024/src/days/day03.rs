use aoc2024::AOC;
pub struct Day03 {}

use nom::{
    character::complete::i64,
    IResult, 
    Parser, 
    multi::fold_many1,
    bytes::complete::{tag,take},
    branch::alt
};

#[derive(Debug, Clone, Copy)]
enum BoolOrInt {
    Int(i64),
    Bool(bool)
}

fn mul_parser(input: &str) -> IResult<&str,BoolOrInt> {
    let (input, _) = tag("mul(")(input)?;
    let (input, a) = i64(input)?;
    let (input, _) = tag(",")(input)?;
    let (input, b) = i64(input)?;
    let (input, _) = tag(")")(input)?;
    Ok((input, BoolOrInt::Int(a * b)))
}

fn do_parser(input: &str) -> IResult<&str,BoolOrInt> {
    let (input, _) = tag("do()")(input)?;
    Ok((input, BoolOrInt::Bool(true)))
}

fn dont_parser(input: &str) -> IResult<&str,BoolOrInt> {
    let (input, _) = tag("don't()")(input)?;
    Ok((input, BoolOrInt::Bool(false)))
}

fn any_0(input: &str) -> IResult<&str,BoolOrInt> {
    let (input, _) = take(1usize)(input)?;
    Ok((input,BoolOrInt::Int(0)))
}

fn mul_or_char(input: &str) -> IResult<&str,BoolOrInt> {
    alt((mul_parser,any_0)).parse(input)
}

fn mul_do_dont_or_char(input: &str) -> IResult<&str,BoolOrInt> {
    alt((do_parser,dont_parser,mul_or_char)).parse(input)
}

fn part_1_parser(input: &str) -> IResult<&str,i64> {
    fold_many1(
        mul_or_char,
        || 0,
        |mut acc, item| { 
            if let BoolOrInt::Int(i) = item { 
                acc += i; 
            } acc 
        }
    ).parse(input)
}

fn part_2_parser(input: &str) -> IResult<&str,(i64,bool)> {
    fold_many1(
        mul_do_dont_or_char,
        || (0, true),
        |mut acc, item| { 
            if let BoolOrInt::Bool(b) = item {
                acc.1 = b;
            } else if let BoolOrInt::Int(i) = item {
                if acc.1 {
                    acc.0 += i;
                }
            }
            acc 
        }
    ).parse(input)
}

impl AOC for Day03 {
    fn part_one(&self, input: &str) -> String {       
        part_1_parser(input).unwrap().1.to_string()
    }

    fn part_two(&self, input: &str) -> String {     
        part_2_parser(input).unwrap().1.0.to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day03.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");

        let my_struct = Day03 {};
        assert_eq!(my_struct.part_one(&file),"161");
    }

    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day03.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");

        let my_struct = Day03 {};
        assert_eq!(my_struct.part_two(&file),"48");
    }
}