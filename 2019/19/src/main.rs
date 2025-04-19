mod intcode;
mod point;

use std::{env,fs};
use intcode::Machine;
use point::Point;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(&file_path).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let mut answer: i64 = 0;

    for y in 0..50 {
        for x in 0..50 {
            if scan(&numbers,x,y) { 
                answer += 1 
            }
        }
    }
    println!("{answer}");

    // part 2
    // assume beam is some kind of angle, but fairly dumb implementation 

    let mut bottom_left: Point<i32> = Point::new(0,100);
    let to_top_left = Point::new(0,-99);
    let to_top_right = Point::new(99,-99);
    let down_a_row = Point::new(0,1);
    let right_a_column = Point::new(1,0);

    loop {
        bottom_left += down_a_row;

        while !scan(&numbers, bottom_left.x, bottom_left.y) {
            bottom_left += right_a_column;
        }

        let top_right= bottom_left + to_top_right;

        if scan(&numbers, top_right.x, top_right.y) {
            let top_left = bottom_left + to_top_left;
            answer = top_left.x as i64 * 10000 + top_left.y as i64;
            println!{"{:?}",top_left};
            break;
        }
    }

    println!("{answer}");

}

fn scan<T> (numbers: &Vec<i64>, x : T, y : T) -> bool  where i64:From<T> {
    let mut machine  = Machine::new(&numbers);
    machine.input.push(x.into());
    machine.input.push(y.into());
    machine.run_to_block();
    machine.output.last().unwrap_or(&0) == &1
}