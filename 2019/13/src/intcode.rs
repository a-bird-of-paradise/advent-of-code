use num_derive::FromPrimitive;
use num_traits::FromPrimitive;
use std::cell::Cell;

// ok have to do some actual work on this to get one machine talking to another 
// define a machine object

pub struct Machine {
    pub input : Vec<i64>,
    pub output : Vec<i64>,
    memory : Vec<i64>,
    instruction_pointer : Cell<usize>, // can be mutated within immutable reference
    relative_base : Cell<i64>
}

// and use emums for opcodes etc so things break properly 

#[derive(FromPrimitive,Debug)] // compiler complains
enum Opcode {
    ADD   =  1,   
    MUL   =  2,  
    READ  =  3,    
    WRITE =  4,  
    JNZ   =  5,   
    JZ    =  6,   
    LT    =  7,   
    EQU   =  8,   
    SETRB =  9,
    HLT   = 99
}

#[derive(FromPrimitive,Debug)]
enum AddressingMode {
    Location = 0,
    Immediate = 1,
    Relative = 2
}

#[derive(PartialEq,Debug)]       // compiler complains this doesn't exist...
pub enum MachineStatus {
    ReadBlocked,
    WriteBlocked,
    Halted
}

impl Machine {
    pub fn new (program : &Vec<i64>) -> Machine {
        Machine {
            input : Vec::new(),
            output : Vec::new(),
            memory : program.clone(),
            instruction_pointer : Cell::new(0),  // "cell" means "it's OK to change"
            relative_base : Cell::new(0)
        }
    }

    fn ip_tick(&self) -> usize {
        let answer : usize = self.instruction_pointer.get();
        self.instruction_pointer.set(answer + 1);
        return answer;
    }

    fn address_modes_from_instruction(instruction : &i64) -> (AddressingMode,AddressingMode,AddressingMode) {
        let param_1_mode : i64 = (instruction /   100) % 10;
        let param_2_mode : i64 = (instruction /  1000) % 10;
        let param_3_mode : i64 = (instruction / 10000) % 10;
        return (
            AddressingMode::from_i64(param_1_mode).expect("Unknown address mode"), 
            AddressingMode::from_i64(param_2_mode).expect("Unknown address mode"), 
            AddressingMode::from_i64(param_3_mode).expect("Unknown address mode")
        );
    }

    fn write_to_memory(&mut self, parameter : usize, mode : AddressingMode, value : i64) {
        match mode {
            AddressingMode::Immediate => panic!("Can not write to an immediate value"),
            AddressingMode::Location => {
                // rust doesn't like looking up with a lookup so have to do it manually
                let index : usize = self.memory[parameter] as usize;
                if index >= self.memory.len() {
                    self.memory.resize(index+1, 0);
                }
                self.memory[index] = value
            },
            AddressingMode::Relative =>  {
                // rust doesn't like looking up with a lookup so have to do it manually
                let index : usize = ((self.memory[parameter]) + self.relative_base.get()).try_into().unwrap();
                if index >= self.memory.len() {
                    self.memory.resize(index+1, 0);
                }
                self.memory[index] = value
            }
        }
    }

    fn read_from_memory(&mut self, parameter : usize, mode : &AddressingMode) -> i64 {

        match mode {
            AddressingMode::Immediate => {
                if parameter >= self.memory.len() {
                    self.memory.resize(parameter+1, 0);
                }
                return self.memory[parameter];
            },
            AddressingMode::Location =>  {
                let pos = self.memory[parameter] as usize;
                if pos >= self.memory.len() {
                    self.memory.resize(pos+1,0);
                }
                return self.memory[self.memory[parameter] as usize]
            },
            AddressingMode::Relative =>  {
                let pos : usize = ((self.memory[parameter]) + self.relative_base.get()).try_into().unwrap();
                if pos >= self.memory.len() {
                    self.memory.resize(pos+1,0);
                }
                return self.memory[pos];
            }
        }
    }

    pub fn run_to_block (&mut self) -> MachineStatus {
        loop {
            let instruction : &i64 = self.memory.get(self.ip_tick()).expect("segfault");
            let opcode : Opcode = Opcode::from_i64(instruction % 100).expect("Unknown instruction");
            let pmodes = Machine::address_modes_from_instruction(instruction);

            match opcode {
                Opcode::ADD => {
                    let param_1: i64 = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    let param_2: i64 = self.read_from_memory(self.ip_tick(), &pmodes.1);

                    self.write_to_memory(self.ip_tick(), pmodes.2, param_1 + param_2);
                },
                Opcode::MUL => {
                    let param_1: i64 = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    let param_2: i64 = self.read_from_memory(self.ip_tick(), &pmodes.1);

                    self.write_to_memory(self.ip_tick(), pmodes.2, param_1 * param_2);
                },
                Opcode::READ => {
                    if self.input.is_empty() { 
                        self.instruction_pointer.set(self.instruction_pointer.get()-1);
                        return MachineStatus::ReadBlocked; // rewind program counter and try again
                     }
                    let input = self.input.remove(0);
                    self.write_to_memory(self.ip_tick(), pmodes.0, input);
                },
                Opcode::WRITE => {
                    let output = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    self.output.push(output);
                    return MachineStatus::WriteBlocked;
                },
                Opcode::JNZ => {
                    let param_1: i64 = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    let param_2: i64 = self.read_from_memory(self.ip_tick(), &pmodes.1);

                    if param_1 != 0 {
                        self.instruction_pointer.set(param_2 as usize);
                    } 
                },
                Opcode::JZ => {
                    let param_1: i64 = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    let param_2: i64 = self.read_from_memory(self.ip_tick(), &pmodes.1);

                    if param_1 == 0 {
                        self.instruction_pointer.set(param_2 as usize);
                    } 
                },
                Opcode::LT => {

                    let param_1: i64 = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    let param_2: i64 = self.read_from_memory(self.ip_tick(), &pmodes.1);

                    let towrite : i64 = if param_1 < param_2 { 1 } else { 0 };

                    self.write_to_memory(self.ip_tick(), pmodes.2, towrite);
                },
                Opcode::EQU => {

                    let param_1: i64 = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    let param_2: i64 = self.read_from_memory(self.ip_tick(), &pmodes.1);

                    let towrite : i64 = if param_1 == param_2 { 1 } else { 0 };

                    self.write_to_memory(self.ip_tick(), pmodes.2, towrite);
                }, 
                Opcode::SETRB => {
                    let param_1 : i64 = self.read_from_memory(self.ip_tick(), &pmodes.0);
                    let current_value = self.relative_base.get();
                    self.relative_base.set(current_value + param_1);
                }
                Opcode::HLT => {
                    return MachineStatus::Halted;
                }
            }
        }
    }

    pub fn memdump (&self) -> Vec<i64> { return self.memory.clone(); }

} 

pub fn do_intcode(
    numbers : Vec<i64>,
    input_tape : &Vec<i64>,
    output_tape : &mut Vec<i64>) -> Vec<i64>
{
    // simple machine driver

    let mut this_machine = Machine::new(&numbers);

    this_machine.input = input_tape.clone();

    let mut status = this_machine.run_to_block();

    while status != MachineStatus::Halted {
        status = this_machine.run_to_block()
    }

    *output_tape = this_machine.output.clone();
    return this_machine.memdump();
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

#[test]
fn test_rel_base() {
    let input_tape : Vec<i64> = Vec::new();
    let mut output_tape : Vec<i64> = Vec::new();

    let numbers : Vec<i64> = vec![109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99];
    do_intcode(numbers.clone(), &input_tape, &mut output_tape);
    assert_eq!(output_tape,numbers);

    let numbers_2 = vec![1102,34915192,34915192,7,4,7,99,0];
    output_tape.clear();    
    do_intcode(numbers_2.clone(), &input_tape, &mut output_tape);
    assert_eq!(*output_tape.last().unwrap(),34915192*34915192);

    let numbers_3 =  vec![104,1125899906842624,99];
    output_tape.clear();    
    do_intcode(numbers_3.clone(), &input_tape, &mut output_tape);
    assert_eq!(*output_tape.last().unwrap(),1125899906842624);
}

#[allow(dead_code)]
pub fn day7_part1_inner(program : &Vec<i64>, perm : &Vec<&i64>) -> i64
{
    let mut machine_a = Machine::new(program);
    machine_a.input.push(*perm[0]);
    machine_a.input.push(0);
    let mut stat = machine_a.run_to_block();
    if stat != MachineStatus::WriteBlocked { panic!("Expected an output"); }

    let mut machine_b = Machine::new(program);
    machine_b.input.push(*perm[1]);
    machine_b.input.push(machine_a.output[0]);
    stat = machine_b.run_to_block();
    if stat != MachineStatus::WriteBlocked { panic!("Expected an output"); }

    let mut machine_c = Machine::new(program);
    machine_c.input.push(*perm[2]);
    machine_c.input.push(machine_b.output[0]);
    stat = machine_c.run_to_block();
    if stat != MachineStatus::WriteBlocked { panic!("Expected an output"); }

    let mut machine_d = Machine::new(program);
    machine_d.input.push(*perm[3]);
    machine_d.input.push(machine_c.output[0]);
    stat = machine_d.run_to_block();
    if stat != MachineStatus::WriteBlocked { panic!("Expected an output"); }

    let mut machine_e = Machine::new(program);
    machine_e.input.push(*perm[4]);
    machine_e.input.push(machine_d.output[0]);
    stat = machine_e.run_to_block();
    if stat != MachineStatus::WriteBlocked { panic!("Expected an output"); }

    return machine_e.output[0];

}

#[allow(dead_code)]
pub fn day7_part2_inner(program : &Vec<i64>, perm : &Vec<&i64>) -> i64
{
    let mut machine_a = Machine::new(program);
    let mut machine_b = Machine::new(program);
    let mut machine_c = Machine::new(program);
    let mut machine_d = Machine::new(program);
    let mut machine_e = Machine::new(program);

    machine_a.input.push(*perm[0]);
    machine_b.input.push(*perm[1]);
    machine_c.input.push(*perm[2]);
    machine_d.input.push(*perm[3]);
    machine_e.input.push(*perm[4]);

    let mut first_time : bool = true;

    loop {
        if first_time { 
            machine_a.input.push(0); 
            first_time = false; 
        } else { 
            machine_a.input.push(*machine_e.output.last().unwrap());
        };
    
        if machine_a.run_to_block() == MachineStatus::Halted { break; }

        machine_b.input.push(*machine_a.output.last().unwrap());
        machine_b.run_to_block();

        machine_c.input.push(*machine_b.output.last().unwrap());
        machine_c.run_to_block();

        machine_d.input.push(*machine_c.output.last().unwrap());
        machine_d.run_to_block();

        machine_e.input.push(*machine_d.output.last().unwrap());
        machine_e.run_to_block();
    }

    return *machine_e.output.last().unwrap();

}


#[test]
fn test_day7_part1_inner() {
    let mut program : Vec<i64> = vec![3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0];
    let mut perm : Vec<&i64> = vec![&4,&3,&2,&1,&0];
    assert_eq!(day7_part1_inner(&program, &perm),43210);

    program = vec![3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0];
    perm = vec![&0,&1,&2,&3,&4];
    assert_eq!(day7_part1_inner(&program, &perm),54321);

    program = vec![3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0];
    perm = vec![&1,&0,&4,&3,&2];
    assert_eq!(day7_part1_inner(&program, &perm),65210);
}

#[test]
fn test_day7_part2_inner() {
    let mut program : Vec<i64> = vec![3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5];
    let mut perm : Vec<&i64> = vec![&9,&8,&7,&6,&5];
    assert_eq!(day7_part2_inner(&program, &perm),139629729);

    program = vec![3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10];
    perm = vec![&9,&7,&8,&5,&6];
    assert_eq!(day7_part2_inner(&program, &perm),18216);
}