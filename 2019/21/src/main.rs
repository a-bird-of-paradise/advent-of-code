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

        part_1(&numbers);
        part_2(&numbers);
    
}

fn part_1(input : &Vec<i64>) {

    let mut machine = Machine::new(&input);
    let mut status : MachineStatus;

    status = machine.run_to_block();

    while status == MachineStatus::WriteBlocked {
        print!("{}",charify(&machine.output, false));
        machine.output.clear();
        status = machine.run_to_block();
    }
/* 
logic
if C is a hole and D is ground, jump  (stored in J)
if A is a hole, jump (stored in T)
*/
    machine.input = intify(
"NOT C T
OR D J
AND T J
NOT A T
OR T J
WALK",true);

    while status != MachineStatus::Halted {
        status = machine.run_to_block();
    }

    print!("{}",charify(&machine.output, false));

}

fn part_2(input : &Vec<i64>) {

    let mut machine = Machine::new(&input);
    let mut status : MachineStatus;

    status = machine.run_to_block();

    while status == MachineStatus::WriteBlocked {
        print!("{}",charify(&machine.output, false));
        machine.output.clear();
        status = machine.run_to_block();
    }
/* 
only jump to D if we can immediately jump to H as well 
also need to test for a hole in B this time too 

: basically
if A is a hole, jump 
if B or C is a hole, and both D and H are safe, jump 
*/
    machine.input = intify("NOT C T
NOT B J
OR T J
AND D J
AND H J
NOT A T
OR T J 
RUN",true);

    while status != MachineStatus::Halted {
        status = machine.run_to_block();
    }

    print!("{}",charify(&machine.output, false));

}

fn intify(input : &str, strip_escape_r : bool) -> Vec<i64>
{
    let mut answer : Vec<i64> = Vec::new();
    for c in input.chars() {
        let to_push = c as i64;
        if strip_escape_r && to_push == 13 {
            continue;
        }
        answer.push(to_push);
    }
    answer.push(10); // '\n'
    answer
}

fn charify(input : &Vec<i64>, strip_escape_r : bool) -> String
{
    let mut answer = String::new();
    for i in input {
        if strip_escape_r && *i == 13 {
            continue;
        }
        if *i >= 256 {
            println!("{}",i);
        }
        let iu8 = *i as u8;
        let ic = iu8 as char;
        answer.push(ic);
    }
    answer
}