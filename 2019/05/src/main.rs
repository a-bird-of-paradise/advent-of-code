use std::fs;
use std::env;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let part1_numbers: Vec<i64> = numbers.clone();
    
    let mut input_tape : Vec<i64> = vec![1];
    let mut output_tape : Vec<i64> = Vec::new();

    do_intcode(part1_numbers, &input_tape, &mut output_tape);

    for i in &output_tape {
        println!("{i}");
    }

    println!("");

    input_tape = vec![5];
    output_tape.clear();

    do_intcode(numbers, &input_tape, &mut output_tape);

    for i in &output_tape {
        println!("{i}");
    }

}

fn do_intcode(
    mut numbers : Vec<i64>,
    input_tape : &Vec<i64>,
    output_tape : &mut Vec<i64>) -> Vec<i64>
{
    let mut instruction_pointer : usize = 0; 
    let mut input_head : usize = 0;

    loop {

        let instruction : i64 = numbers[instruction_pointer];

        let opcode : i64 = instruction % 100;
        let param_1_mode : i64 = (instruction /   100) % 10;
        let param_2_mode : i64 = (instruction /  1000) % 10;
        let param_3_mode : i64 = (instruction / 10000) % 10;

        match opcode {
            1 => {
                let operand_1 : i64; 
                let operand_2 : i64;

                if param_1_mode == 0 {
                    let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                    operand_1 = numbers[operand1_location];
                } else if param_1_mode == 1 {
                    operand_1 = numbers[instruction_pointer + 1];
                } else {
                    panic!("Unknown address mode");
                }

                if param_2_mode == 0 {
                    let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                    operand_2 = numbers[operand2_location];
                } else if param_2_mode == 1 {
                    operand_2 = numbers[instruction_pointer + 2];
                } else {
                    panic!("Unknown address mode");
                }
                
                if param_3_mode == 1 { panic!("Can only store to an address"); }

                let result_location : usize = numbers[instruction_pointer + 3].try_into().unwrap();
                numbers[result_location] = operand_1 + operand_2; 
                instruction_pointer += 4;
            },
            2 => {
                let operand_1 : i64; 
                let operand_2 : i64;

                if param_1_mode == 0 {
                    let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                    operand_1 = numbers[operand1_location];
                } else if param_1_mode == 1 {
                    operand_1 = numbers[instruction_pointer + 1];
                } else {
                    panic!("Unknown address mode");
                }

                if param_2_mode == 0 {
                    let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                    operand_2 = numbers[operand2_location];
                } else if param_2_mode == 1 {
                    operand_2 = numbers[instruction_pointer + 2];
                } else {
                    panic!("Unknown address mode");
                }
                
                if param_3_mode == 1 { panic!("Can only store to an address"); }

                let result_location : usize = numbers[instruction_pointer + 3].try_into().unwrap();
                numbers[result_location] = operand_1 * operand_2; 
                instruction_pointer += 4;
            },
            3 => {
                if param_1_mode != 0 { panic!("Can only read into an address"); }
                let result_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                numbers[result_location] = input_tape[input_head];
                input_head += 1;
                instruction_pointer += 2;
            }
            4 => {
                if param_1_mode == 0 {
                    let result_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                    output_tape.push(numbers[result_location]);
                } else if param_1_mode == 1 {
                    output_tape.push(numbers[instruction_pointer+1]);
                } else {
                    panic!("Invalid address mode");
                }
                instruction_pointer += 2;
            },
            5 => {
                let operand_1 : i64; 
                let operand_2 : i64;

                if param_1_mode == 0 {
                    let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                    operand_1 = numbers[operand1_location];
                } else if param_1_mode == 1 {
                    operand_1 = numbers[instruction_pointer + 1];
                } else {
                    panic!("Unknown address mode");
                }

                if param_2_mode == 0 {
                    let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                    operand_2 = numbers[operand2_location];
                } else if param_2_mode == 1 {
                    operand_2 = numbers[instruction_pointer + 2];
                } else {
                    panic!("Unknown address mode");
                }

                if operand_1 != 0 {
                    instruction_pointer = operand_2 as usize;
                } else {
                    instruction_pointer += 3;
                } 
            },
            6 => {
                let operand_1 : i64; 
                let operand_2 : i64;

                if param_1_mode == 0 {
                    let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                    operand_1 = numbers[operand1_location];
                } else if param_1_mode == 1 {
                    operand_1 = numbers[instruction_pointer + 1];
                } else {
                    panic!("Unknown address mode");
                }

                if param_2_mode == 0 {
                    let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                    operand_2 = numbers[operand2_location];
                } else if param_2_mode == 1 {
                    operand_2 = numbers[instruction_pointer + 2];
                } else {
                    panic!("Unknown address mode");
                }

                if operand_1 == 0 {
                    instruction_pointer = operand_2 as usize;
                } else {
                    instruction_pointer += 3;
                } 
            },
            7 => {
                let operand_1 : i64; 
                let operand_2 : i64;

                if param_1_mode == 0 {
                    let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                    operand_1 = numbers[operand1_location];
                } else if param_1_mode == 1 {
                    operand_1 = numbers[instruction_pointer + 1];
                } else {
                    panic!("Unknown address mode");
                }

                if param_2_mode == 0 {
                    let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                    operand_2 = numbers[operand2_location];
                } else if param_2_mode == 1 {
                    operand_2 = numbers[instruction_pointer + 2];
                } else {
                    panic!("Unknown address mode");
                }
                
                if param_3_mode == 1 { panic!("Can only store to an address"); }

                let result_location : usize = numbers[instruction_pointer + 3].try_into().unwrap();

                numbers[result_location] = match operand_1 < operand_2 {
                    true => 1,
                    false => 0,
                };
                
                instruction_pointer += 4;
            },
            8 => {
                let operand_1 : i64; 
                let operand_2 : i64;

                if param_1_mode == 0 {
                    let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                    operand_1 = numbers[operand1_location];
                } else if param_1_mode == 1 {
                    operand_1 = numbers[instruction_pointer + 1];
                } else {
                    panic!("Unknown address mode");
                }

                if param_2_mode == 0 {
                    let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                    operand_2 = numbers[operand2_location];
                } else if param_2_mode == 1 {
                    operand_2 = numbers[instruction_pointer + 2];
                } else {
                    panic!("Unknown address mode");
                }
                
                if param_3_mode == 1 { panic!("Can only store to an address"); }

                let result_location : usize = numbers[instruction_pointer + 3].try_into().unwrap();

                numbers[result_location] = match operand_1 == operand_2 {
                    true => 1,
                    false => 0,
                };
                
                instruction_pointer += 4;
            },
            99 => break,
            _ => panic!("Unknown opcode")
        }
    }
    return numbers;
}

#[test]
fn test_do_intcode() {
    let input_tape : Vec<i64> = Vec::new();
    let mut output_tape : Vec<i64> = Vec::new();
    assert_eq!(do_intcode(vec![1,0,0,0,99], &input_tape, &mut output_tape), vec![2,0,0,0,99]);
    assert_eq!(do_intcode(vec![2,3,0,3,99], &input_tape, &mut output_tape), vec![2,3,0,6,99]);
    assert_eq!(do_intcode(vec![2,4,4,5,99,0], &input_tape, &mut output_tape), vec![2,4,4,5,99,9801]);
    assert_eq!(do_intcode(vec![1,1,1,4,99,5,6,0,99], &input_tape, &mut output_tape), vec![30,1,1,4,2,5,6,0,99]);
}

#[test]
fn test_immediate_arg() {
    let input_tape : Vec<i64> = Vec::new();
    let mut output_tape : Vec<i64> = Vec::new();
    assert_eq!(do_intcode(vec![1002,4,3,4,33], &input_tape, &mut output_tape), vec![1002,4,3,4,99]);
}

#[test]
fn test_rw() {
    let input_tape : Vec<i64> = vec![12345];
    let mut output_tape : Vec<i64> = Vec::new();
    assert_eq!(do_intcode(vec![3,0,4,0,99], &input_tape, &mut output_tape), vec![12345,0,4,0,99]);
    assert_eq!(output_tape[0],12345);
}

#[test]
fn test_jumps() {
    let mut input_tape : Vec<i64> = vec![0];
    let mut output_tape : Vec<i64> = Vec::new();
    assert_eq!(do_intcode(vec![3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9], &input_tape, &mut output_tape), vec![3,12,6,12,15,1,13,14,13,4,13,99,0,0,1,9]);
    assert_eq!(*output_tape.last().unwrap(),0);
    do_intcode(vec![3,3,1105,-1,9,1101,0,0,12,4,12,99,1], &input_tape, &mut output_tape);
    assert_eq!(*output_tape.last().unwrap(),0);

    input_tape = vec![12345];
    assert_eq!(do_intcode(vec![3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9], &input_tape, &mut output_tape), vec![3,12,6,12,15,1,13,14,13,4,13,99,12345,1,1,9]);
    assert_eq!(*output_tape.last().unwrap(),1);
    do_intcode(vec![3,3,1105,-1,9,1101,0,0,12,4,12,99,1], &input_tape, &mut output_tape);
    assert_eq!(*output_tape.last().unwrap(),1);

}

#[test]
fn test_d5p2() {
    let mut input_tape : Vec<i64> = vec![0];
    let mut output_tape : Vec<i64> = Vec::new();

    let numbers : Vec<i64> = vec![3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99];

    do_intcode(numbers.clone(), &input_tape, &mut output_tape);
    assert_eq!(*output_tape.last().unwrap(), 999);

    input_tape = vec![8];
    do_intcode(numbers.clone(), &input_tape, &mut output_tape);
    assert_eq!(*output_tape.last().unwrap(), 1000);

    input_tape = vec![8000];
    do_intcode(numbers.clone(), &input_tape, &mut output_tape);
    assert_eq!(*output_tape.last().unwrap(), 1001);

}