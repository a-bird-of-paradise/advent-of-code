use aoc2024::AOC;
mod days;
use std::path::PathBuf;
use std::fs;
use structopt::StructOpt;

#[derive(Debug,StructOpt)]
#[structopt(name = "Advent of Code 2024", about = "Driver for AoC 2024 solutions")]
struct Opt {
    #[structopt(short,long)]
    day: Option<usize>,
    
    #[structopt(parse(from_os_str))]
    input: Option<PathBuf>,

    #[structopt(short,long,conflicts_with("day"))]
    all: bool
}

fn default_file_name(day: usize) -> PathBuf {
    PathBuf::from(format!("inputs/day{:02}.txt", day))
}

fn run_day_inner(day:usize, input: &str) -> (String,String) {
    (
        day_to_problem(day).unwrap().part_one(input),
        day_to_problem(day).unwrap().part_two(input)
    )
}

fn run_day(day:usize, file_name: &PathBuf) -> (String,String) {
    let input = fs::read_to_string(file_name).unwrap();
    run_day_inner(day, &input)
}

fn main() {
    let opt = Opt::from_args();

    // do one day

    if opt.day.is_some() {
        let day = opt.day.unwrap();
        let file_name = if opt.input.is_none() { default_file_name(day) } else { opt.input.unwrap() };

        let (part1,part2) = run_day(day, &file_name);
        println!("Part 1: {part1}");
        println!("Part 2: {part2}");
    }

    if opt.all {

        for i in 1..=15 {
            let (part1,part2) = run_day(i,&default_file_name(i));
            println!("Day {i:02}, Part 1:  {part1}");
            println!("Day {i:02}, Part 2:  {part2}");
        }

    }
}

fn day_to_problem(day: usize) -> Option<Box<dyn AOC>> {
    match day {
        1 => Some(Box::new(days::day01::Day01{})),
        2 => Some(Box::new(days::day02::Day02{})),
        3 => Some(Box::new(days::day03::Day03{})),
        4 => Some(Box::new(days::day04::Day04{})),
        5 => Some(Box::new(days::day05::Day05{})),
        6 => Some(Box::new(days::day06::Day06{})),
        7 => Some(Box::new(days::day07::Day07{})),
        8 => Some(Box::new(days::day08::Day08{})),
        9 => Some(Box::new(days::day09::Day09{})),
        10=> Some(Box::new(days::day10::Day10{})),
        11=> Some(Box::new(days::day11::Day11{})),
        12=> Some(Box::new(days::day12::Day12{})),
        13=> Some(Box::new(days::day13::Day13{})),
        14=> Some(Box::new(days::day14::Day14{})),
        15=> Some(Box::new(days::day15::Day15{})),
        _ => None
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test_default_file_name() {
        assert_eq!(default_file_name(9).display().to_string(), "inputs/day09.txt");
        assert_eq!(default_file_name(20).display().to_string(), "inputs/day20.txt");
    }
}