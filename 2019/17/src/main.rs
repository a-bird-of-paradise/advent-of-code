mod intcode;

use core::num;
use std::{env,fs};
use intcode::{do_intcode, Machine, MachineStatus};

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let input_tape : Vec<i64> = Vec::new();
    let mut output_tape : Vec<i64> = Vec::new();

    do_intcode(numbers, &input_tape, &mut output_tape);

    assert_eq!(output_tape.len(), 37 * 61);
    
    let n_row = 48usize;
    let n_col = 48usize;

    let mut answer : usize = 0;

    for row in 1..n_row-2 {
        for col in 1..n_col - 2 {

            if output_tape[(row + 0 ) * n_col + col - 1] != 46
            && output_tape[(row + 0 ) * n_col + col + 1] != 46
            && output_tape[(row + 1 ) * n_col + col + 0] != 46
            && output_tape[(row - 1 ) * n_col + col + 0] != 46 
            && output_tape[(row + 0 ) * n_col + col + 0] != 46 {
                println!("!!{row} {col}");
                output_tape[row * n_col + col] = 79;
                answer += row * col;
            }
        }
    }
    print_tape(&output_tape);

    println!("{answer}");

}

fn print_tape(tape : &Vec<i64>) 
{
    let mut ticker = 0;
    let mut rowcount = 0;
    print!("{:0>4} ",ticker); 
    for i in tape.iter() {
        let c : char;
        c = match i {
            46 => '.',
            35 => '#',
            10 => '\n',
            86 => 'V',
            76 => 'v',
            60 => '<',
            62 => '>',
            94 => '^',
            79 => 'O',
            _ => '*'
        };
        print!("{}",c);
        ticker += 1;
        if *i == 10 { print!("{:0>4} ",ticker); rowcount += 1; }
    }
    println!("{rowcount}");
}