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

    let mut part1_numbers = numbers.clone();

    if file_path == "input.txt" {
        part1_numbers[1] = 12;
        part1_numbers[2] = 2;
    }

    let answer_1 = do_intcode(part1_numbers)[0];

    println!("{answer_1}");

    for noun in 0..99 {
        for verb in 0..99 {
            let mut part2_numbers = numbers.clone(); 
            part2_numbers[1] = noun;
            part2_numbers[2] = verb;
            let answer_2 = do_intcode(part2_numbers)[0];
            if answer_2 == 19690720  {
                let noun_f = format!("{:0>2}",noun);
                let verb_f = format!("{:0>2}",verb);
                println!("{noun_f}{verb_f}");
            }
        }
    }
}

fn do_intcode(mut numbers : Vec<i64>) -> Vec<i64>
{
    let mut instruction_pointer : usize = 0; 

    loop {

        let opcode = numbers[instruction_pointer];

        match opcode {
            1 => {
                let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                let result_location : usize = numbers[instruction_pointer + 3].try_into().unwrap();
                numbers[result_location] = numbers[operand1_location] + numbers[operand2_location];
                instruction_pointer += 4;
            },
            2 => {
                let operand1_location : usize = numbers[instruction_pointer + 1].try_into().unwrap();
                let operand2_location : usize = numbers[instruction_pointer + 2].try_into().unwrap();
                let result_location : usize = numbers[instruction_pointer + 3].try_into().unwrap();
                numbers[result_location] = numbers[operand1_location] * numbers[operand2_location];
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
    assert_eq!(do_intcode(vec![1,0,0,0,99]), vec![2,0,0,0,99]);
    assert_eq!(do_intcode(vec![2,3,0,3,99]), vec![2,3,0,6,99]);
    assert_eq!(do_intcode(vec![2,4,4,5,99,0]), vec![2,4,4,5,99,9801]);
    assert_eq!(do_intcode(vec![1,1,1,4,99,5,6,0,99]), vec![30,1,1,4,2,5,6,0,99]);
}