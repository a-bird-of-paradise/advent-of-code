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

fn main() {
    let opt = Opt::from_args();
    let problem = day_to_problem(opt.day).unwrap();
    let file = fs::read_to_string(opt.input.unwrap()).unwrap();

    println!("Part 1: {}", problem.part_one(&file));
    println!("Part 2: {}", problem.part_two(&file));
}

fn day_to_problem(day: usize) -> Option<Box<dyn AOC>> {
    match day {
        1 => Some(Box::new(days::day01::Day01{})),
        _ => None
    }
}