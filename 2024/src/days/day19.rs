
use std::{collections::HashMap};

use aoc2024::AOC;
use nom::{bytes::complete::{tag, take_while}, character::complete::line_ending, multi::separated_list1, sequence::{separated_pair, terminated}, AsChar, IResult, Parser};

pub struct Day19;

fn read_towels(input: &str) -> IResult<&str,Vec<&str>>
{
    terminated(separated_list1(tag(", "), take_while(AsChar::is_alpha)),line_ending).parse(input)
}

fn read_designs(input: &str) -> IResult<&str,Vec<&str>>
{
    separated_list1(line_ending, take_while(AsChar::is_alpha)).parse(input)
}

fn read_file(input: &str) -> IResult<&str, (Vec<&str>, Vec<&str>)>
{
    separated_pair(read_towels, line_ending, read_designs).parse(input)
}

fn producable<'a>(design: &'a str, patterns: &'a [&'a str], cache: &mut HashMap<String,bool>) -> bool {
    if design.is_empty() { 
        true
    } else if cache.contains_key(design) {
        *cache.get(design).unwrap()
    } else { 
        let answer = patterns
            .iter()
            .filter(|pattern| design.starts_with(*pattern))
            .any(|pattern| producable(&design[pattern.len()..], patterns, cache));
        cache.entry(design.to_string()).or_insert(answer);
        answer
    }
}

fn countable<'a>(design: &'a str, patterns: &'a [&'a str], cache: &mut HashMap<String,usize>) -> usize {
    if design.is_empty() { 
        1
    } else if cache.contains_key(design) {
        *cache.get(design).unwrap()
    } else { 
        let answer = patterns
            .iter()
            .filter(|pattern| design.starts_with(*pattern))
            .map(|pattern| countable(&design[pattern.len()..], patterns, cache))
            .sum();
        cache.entry(design.to_string()).or_insert(answer);
        answer
    }
}

impl AOC for Day19 {
    fn part_one(&self, input: &str) -> String {
        let (patterns, designs) = read_file(input).unwrap().1;

        let mut cache: HashMap<String,bool> = Default::default();

        designs
            .iter()
            .filter(|design| producable(design, &patterns, &mut cache))
            .count()
            .to_string()

    }

    fn part_two(&self, input: &str) -> String {
        let (patterns, designs) = read_file(input).unwrap().1;

        let mut cache: HashMap<String,usize> = Default::default();

        designs
            .iter()
            .map(|design| countable(design, &patterns, &mut cache))
            .sum::<usize>()
            .to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day19.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day19 {};
        assert_eq!(my_struct.part_one(&file),"6");
    }

    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day19.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day19 {};
        assert_eq!(my_struct.part_two(&file),"16");
    }

    #[test]
    fn test_parser() {
        let test_file_path = "test-inputs/day19.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let (a,b) = read_file(&file).unwrap().1;
        assert_eq!(a,vec!["r", "wr", "b", "g", "bwu", "rb", "gb", "br"]);
        assert_eq!(b,vec!["brwrr", "bggr", "gbbr", "rrbgbr", "ubwu", "bwurrg", "brgr", "bbrgwb"]);
    }
}