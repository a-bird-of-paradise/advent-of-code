use aoc2024::AOC;
use nom::{bytes::complete::tag, character::{complete::{anychar, u64, line_ending}}, combinator::opt, multi::separated_list1, IResult, Parser};
use num_derive::FromPrimitive;
use num_traits::FromPrimitive;

pub struct Day17;

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct Machine {
    pub a: u64, 
    pub b: u64, 
    pub c: u64, 
    pub mem: Vec<u64>,
    pub i: usize
}

#[allow(non_camel_case_types)]
#[derive(FromPrimitive,Debug)] // compiler complains
enum Opcode {
    adv = 0,
    bxl = 1, 
    bst = 2,
    jnz = 3,
    bxc = 4,
    out = 5,
    bdv = 6,
    cdv = 7
}

impl Machine {
    fn run(&mut self, out_tape: &mut Vec<u64>) {
        while self.i < self.mem.len() {
            let opcode = Opcode::from_u64(self.mem[self.i]).expect("Could not convert");
            //println!("{}: {:?} {} {} {} {}",self.i,opcode,self.a,self.b,self.c,self.mem[self.i+1]);
            self.i += 1;
            match opcode {
                Opcode::adv => {
                    self.a = self.a  / 2u64.pow(self.combo_operand_value().try_into().unwrap());
                    self.i += 1;
                },
                Opcode::bdv => {
                    self.b = self.a  / 2u64.pow(self.combo_operand_value().try_into().unwrap());
                    self.i += 1;
                },
                Opcode::cdv => {
                    self.c = self.a  / 2u64.pow(self.combo_operand_value().try_into().unwrap());
                    self.i += 1;
                },
                Opcode::bxl => {
                    self.b ^= self.operand_value();
                    self.i += 1;
                },
                Opcode::bxc => {
                    self.b ^= self.c;
                    self.i += 1;
                },
                Opcode::bst => {
                    self.b = self.combo_operand_value().rem_euclid(8);
                    self.i += 1;
                },
                Opcode::jnz => {
                    if self.a == 0 { 
                        self.i += 1 ;
                    } else {
                        self.i = self.operand_value().try_into().unwrap() ;
                    }
                },
                Opcode::out => {
                    out_tape.push(self.combo_operand_value().rem_euclid(8));
                    self.i += 1;
                }
            }
        }
    }

    fn combo_operand_value(&self) -> u64 {
        match self.mem[self.i] {
            0..=3   => { self.mem[self.i]   },
            4       => { self.a             },
            5       => { self.b             },
            6       => { self.c             },
            _       => { panic!{"Oh no"}    }
        }
    }

    fn operand_value(&self) -> u64 {
        self.mem[self.i]
    }
}

fn read_register (input: &str) -> IResult<&str,u64> {
    let (input, (_,_,_,i,_)) = (tag("Register "), anychar, tag(": "), u64,line_ending).parse(input)?;
    Ok((input,i))
}

fn read_program (input: &str) -> IResult<&str, Vec<u64>> {
    let (input, (_, v, _)) = (tag("Program: "), separated_list1(tag(","), u64), opt(line_ending)).parse(input)?;
    Ok((input,v))
}

fn read_input (input: &str) -> IResult<&str,(u64,u64,u64,Vec<u64>)> {
    let (input, (a,b,c,_,v)) = (read_register,read_register,read_register,line_ending,read_program).parse(input)?;
    Ok((input,(a,b,c,v)))
}

fn digits_to_value(digits: &[u64]) -> u64 {
    let mut answer = 0;

    if digits.len() > 0 {
        let mut place = 8u64.pow(((digits.len()-1) as u32).try_into().unwrap());

        for digit in digits {
            answer += place * digit;
            place /= 8;
        }
    }

    answer
}

#[allow(dead_code)]
fn value_to_digits(value: u64) -> Vec<u64> {
    let mut answer: Vec<u64> = Default::default();
    let mut value = value;

    if value == 0 { answer.push(0); }

    while value > 0 { 
        answer.push(value.rem_euclid(8));
        value /= 8;
    }

    answer.iter().copied().rev().collect()
}

impl AOC for Day17{

    fn part_one(&self, input: &str) -> String {

        let (a,b,c,v) = read_input(input).unwrap().1;

        let mut machine = Machine { a: a, b: b, c: c, mem: v, i: 0};
        let mut out_tape = Vec::<u64>::new();

        machine.run(&mut out_tape);

        let answer = out_tape.iter().map(|u| u.to_string() + ",").collect::<String>();
        let answer = answer.trim_end_matches(",");

        answer.to_string()
    }

    fn part_two(&self, input: &str) -> String {    
        
        let (_, b,c,v) = read_input(input).unwrap().1;
        let rev_v: Vec<_> = v.iter().copied().rev().collect();

        let mut candidates : Vec<Vec<u64>> = Default::default(); // holds octal digits
        let mut answers : Vec<Vec<u64>> = Default::default();

        for i in 0..8u64 {candidates.push(vec![i])}
        let mut out_tape = Vec::<u64>::new();

        while !candidates.is_empty() {
            let current_candidate = candidates.pop().unwrap();
            if current_candidate.len() > rev_v.len() { continue }
            let candidate = digits_to_value(&current_candidate);

            let mut machine = Machine { a: candidate, b: b, c: c, mem: v.clone(), i: 0};
            out_tape.clear();
            machine.run(&mut out_tape);
            let out_tape_rev : Vec<_> = out_tape.iter().copied().rev().collect();

            if out_tape_rev == &rev_v[0..out_tape_rev.len()] {

                if out_tape_rev == rev_v {
                    answers.push(current_candidate.clone());
                }

                for i in 0..8u64 {
                    let mut temp = current_candidate.clone();
                    temp.push(i);
                    candidates.push(temp);
                }
            }
        }

        answers
            .iter()
            .map(|a| digits_to_value(&a))
            .min()
            .unwrap()
            .to_string()

    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day17.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day17 {};
        assert_eq!(my_struct.part_one(&file),"4,6,3,5,6,3,5,2,1,0");
    }

    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day17.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day17 {};
        assert_eq!(my_struct.part_two(&file),"117440");
    }

    #[test]
    fn test_digits_to_value() {
        assert_eq!(digits_to_value(&vec![0]),0);
        assert_eq!(digits_to_value(&vec![1]),1);
        assert_eq!(digits_to_value(&vec![1,0]),8);
        assert_eq!(digits_to_value(&vec![3,2,1]),1 + 2*8 + 3*8*8);
    }

    #[test]
    fn test_value_to_digits() {
        assert_eq!(value_to_digits(3*8*8 + 2*8 + 1),vec![3,2,1]);
        assert_eq!(value_to_digits(1),vec![1]);
        assert_eq!(value_to_digits(0),vec![0]);
        assert_eq!(value_to_digits(8),vec![1,0]);
    }

}


        /*
        test program is 0,1,5,4,3,0
        while a > 0 { 
            a /= 2; 
            write(a % 8);
        }
        
        second test is 0,3,5,4,3,0
        while a > 0 
            a /= 8;
            write a % 8; 

        ok search time

        for each stem 
            make candidates 
            for each candidate 
            if could be 
                next stem 

         */
