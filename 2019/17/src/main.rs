mod intcode;

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

    do_intcode(&numbers, &input_tape, &mut output_tape);

    let answer = part_1(&output_tape);

    println!("Part 1: {answer}");

    let routine = intify("A,B,B,C,B,C,B,C,A,A");
    let func_a = intify("L,6,R,8,L,4,R,8,L,12");
    let func_b = intify("L,12,R,10,L,4");
    let func_c = intify("L,12,L,6,L,4,L,4");
    let output_choice = intify("n");

    let input_tape_2 = vec![routine,func_a,func_b,func_c,output_choice].concat(); // 'y', '\n'

    let program = part_2_ify(&numbers);
    
    let mut machine : Machine = Machine::new(&program);
    let mut status : MachineStatus; 
    output_tape.clear();

    machine.input = input_tape_2;

    status = machine.run_to_block();

    while status != MachineStatus::Halted {
        status = machine.run_to_block();
    }

    println!("Part 2: {:?}",machine.output.last().unwrap());

}

fn part_2_ify(numbers : &Vec<i64>) -> Vec<i64>
{
    let mut answer = numbers.clone();
    answer[0] = 2;
    answer
}

fn intify(input : &str) -> Vec<i64>
{
    let mut answer : Vec<i64> = Vec::new();
    for c in input.chars() {
        answer.push(c as i64);
    }
    answer.push(10); // '\n'
    answer
}

fn part_1(output_tape : &Vec<i64>) -> usize
{
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
                //output_tape[row * n_col + col] = 79;
                answer += row * col;
            }
        }
    }
    print_tape(&output_tape);
    answer
}

fn print_tape(tape : &Vec<i64>) 
{
    let mut ticker = 0;
    print!("{:0>4} ",ticker); 
    for i in tape.iter() {
        let c : char =print_tape_inner(*i);
        print!("{}",c);
        ticker += 1;
        if *i == 10 { print!("{:0>4} ",ticker); }
    }
    println!("");
}

fn print_tape_inner(i: i64) -> char {
    (i as u8) as char
}