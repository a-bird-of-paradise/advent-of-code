use aoc2024::AOC;
mod days;
use std::path::PathBuf;
use std::fs;
use structopt::StructOpt;

#[derive(Debug,StructOpt)]
#[structopt(name = "Advent of Code 2024", about = "Driver for AoC 2024 solutions")]
struct Opt {
    #[structopt(short,long)]
    day: usize,
    
    #[structopt(parse(from_os_str))]
    input: Option<PathBuf>
}

fn default_file_name(day: usize) -> PathBuf {
    PathBuf::from(format!("inputs/day{:02}.txt", day))
}

fn main() {
    let opt = Opt::from_args();
    let problem = day_to_problem(opt.day).unwrap();

    let file_name = if opt.input.is_none() { default_file_name(opt.day) } else { opt.input.unwrap() };

    let file = fs::read_to_string(file_name).unwrap();

    println!("Part 1: {}", problem.part_one(&file));
    println!("Part 2: {}", problem.part_two(&file));
}

fn day_to_problem(day: usize) -> Option<Box<dyn AOC>> {
    match day {
        1 => Some(Box::new(days::day01::Day01{})),
        2 => Some(Box::new(days::day02::Day02{})),
        3 => Some(Box::new(days::day03::Day03{})),
        4 => Some(Box::new(days::day04::Day04{})),
        5 => Some(Box::new(days::day05::Day05{})),
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