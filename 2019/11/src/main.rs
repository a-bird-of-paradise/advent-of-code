use core::num;
use std::fs;
use std::env;
mod intcode;
mod point;
use std::collections::BTreeSet;
use intcode::MachineStatus;
use point::Point;
use num_derive::FromPrimitive;

#[derive(PartialEq,FromPrimitive,Copy,Clone,Debug)] 
enum Orientation 
{
    North = 0,
    East = 1,
    South = 2,
    West = 3
}

#[derive(Debug)]
struct Facing {
    pub orientation : Orientation
}

impl Facing {
    pub fn new(direction: &Orientation) -> Facing { Facing { orientation: *direction } }
    pub fn rotate_left(&mut self) { 
        self.orientation = match self.orientation {
            Orientation::North => Orientation::West,
            Orientation::East => Orientation::North,
            Orientation::South => Orientation::East,
            Orientation::West => Orientation::South
        }
    }
    pub fn rotate_right(&mut self) { 
        self.orientation = match self.orientation {
            Orientation::North => Orientation::East,
            Orientation::East => Orientation::South,
            Orientation::South => Orientation::West,
            Orientation::West => Orientation::North
        }
    }
}


fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();
    
    let part_1_output = inner(&numbers,false);
    println!("{}",part_1_output.0.len()); 

    let part_2_output = inner(&numbers,true);
    print_painted(&part_2_output.1);
}

pub fn print_painted(to_print: &BTreeSet<Point>) {
    let xmin: i32 = to_print.iter().map(|p| p.x).min().unwrap();
    let xmax: i32 = to_print.iter().map(|p| p.x).max().unwrap();
    let ymin: i32 = to_print.iter().map(|p| p.y).min().unwrap();
    let ymax: i32 = to_print.iter().map(|p| p.y).max().unwrap();

    let xrange = xmax - xmin + 1;
    let yrange = ymax - ymin + 1;

    for j in 0..yrange {
        for i in 0..xrange {
            let c: char = if to_print.contains(&Point::new(xmin + i, ymax - j)) {'#'} else {' '};
            print!("{c}");
        }
        print!("\n");
    }
}

pub fn inner(program: &Vec<i64>, initial_white: bool) -> (BTreeSet<Point>,BTreeSet<Point>) {
    
    let mut visited : BTreeSet<Point> = BTreeSet::new(); 
    let mut currently_painted : BTreeSet<Point> = BTreeSet::new();

    let mut location :  (Point, Facing) = ( Point::new(0,0), Facing::new(&Orientation::North));
    let mut machine : intcode::Machine = intcode::Machine::new(program);

    let mut status: MachineStatus;

    if initial_white { currently_painted.insert(Point::new(0,0)); }

    loop {
        status = machine.run_to_block();
        match status {
            MachineStatus::Halted => break,
            MachineStatus::ReadBlocked => {
                if currently_painted.contains(&location.0) {
                    machine.input.push(1);
                } else {
                    machine.input.push(0);
                }
            },
            MachineStatus::WriteBlocked => {
                if *machine.output.last().unwrap() == 0 {
                    currently_painted.remove(&location.0);
                } else {
                    visited.insert(location.0);
                    currently_painted.insert(location.0);
                }
                
                let inner_status: MachineStatus = machine.run_to_block();
                assert_eq!(inner_status,MachineStatus::WriteBlocked);

                if *machine.output.last().unwrap() == 0 {
                    location.1.rotate_left();
                } else {
                    location.1.rotate_right();
                }

                match location.1.orientation {
                    Orientation::North => location.0.y += 1,
                    Orientation::East  => location.0.x += 1,
                    Orientation::South => location.0.y -= 1,
                    Orientation::West  => location.0.x -= 1,
                }
            }
        }
    }

    return (visited,currently_painted);

}

