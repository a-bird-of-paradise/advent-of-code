mod intcode;

use std::{env, fs::{self, read}, i64, io};
use intcode::{Machine, MachineStatus};
use num_traits::pow;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let mut machine = Machine::new(&numbers);

    let items = vec!["antenna","space heater","jam","fixed point","tambourine","festive hat","easter egg","asterisk"];

    let all_combinations = powerset(&items);

    read_from_machine(&mut machine);

    for item in &items { get_item(&mut machine, item); }

    next_to_checkpoint(&mut machine);

/*
    loop {
        let mut msg = String::new();
        io::stdin().read_line(&mut msg).expect("Input failure");

        if msg.trim() == "exit" {
            break;
        }

        write_to_machine(&mut machine, &msg.trim());
        read_from_machine(&mut machine);
    }

    */

    for combo in all_combinations {
        drop_all_items(&mut machine, &items);
        for item in combo {
            take_item(&mut machine, item);
        }
        go_west(&mut machine);
    }

}

fn powerset<T: Clone>(s: &[T]) -> Vec<Vec<T>> {
    let mut subsets: Vec<Vec<T>> = vec![];
    subsets.push(vec![]);

    let mut updated: Vec<Vec<T>> = vec![]; 

    for ele in s {
        for mut sub in subsets.clone() {
            sub.push(ele.clone());
            updated.push(sub);
        }
        subsets.append(&mut updated);
    }
    subsets
}


fn read_from_machine(machine: &mut Machine) {
    let mut status = machine.run_to_block();
    while status == MachineStatus::WriteBlocked {
        status = machine.run_to_block();
    }
    for c in &machine.output {
        print!("{}",(*c as u8) as char);
    }
    machine.output.clear();
}

fn write_to_machine(machine: &mut Machine, msg: &str) {
    for c in msg.chars() {
        machine.input.push(c as i64);
    }
    machine.input.push(10);
}

fn take_item(machine: &mut Machine, item: &str) {
    let mut cmd = String::new();
    cmd.push_str("take ");
    cmd.push_str(item);
    write_to_machine(machine, &cmd);
    read_from_machine(machine);
}

fn drop_all_items(machine: &mut Machine, items: &Vec<&str>) {
    for item in items {
        let mut cmd = String::new();
        cmd.push_str("drop ");
        cmd.push_str(&item);
        write_to_machine(machine, &cmd);
        read_from_machine(machine);
    }
}

fn go_north(machine: &mut Machine) {
    write_to_machine(machine, "north".trim());    
    let mut status = machine.run_to_block();
    while status == MachineStatus::ReadBlocked {
        status = machine.run_to_block();
    }
}

fn go_south(machine: &mut Machine) {
    write_to_machine(machine, "south".trim());
    let mut status = machine.run_to_block();
    while status == MachineStatus::ReadBlocked {
        status = machine.run_to_block();
    }
}

fn go_east(machine: &mut Machine) {
    write_to_machine(machine, "east".trim());
    let mut status = machine.run_to_block();
    while status == MachineStatus::ReadBlocked {
        status = machine.run_to_block();
    }
}

fn go_west(machine: &mut Machine) {
    write_to_machine(machine, "west".trim());
    let mut status = machine.run_to_block();
    while status == MachineStatus::ReadBlocked {
        status = machine.run_to_block();
    }
}

fn next_to_checkpoint(machine: &mut Machine) {
    go_south(machine);
    for _i in 0..=3 { go_west(machine); }
}

fn get_item(machine: &mut Machine, item: &str) {
    if item == "space heater" {
        go_south(machine);
        go_west(machine);
        go_west(machine);
        go_west(machine);
        take_item(machine, item);
        go_east(machine);
        go_east(machine);
        go_east(machine);
        go_north(machine);
    } else if item == "antenna" {
        go_south(machine);
        go_west(machine);
        go_south(machine);
        take_item(machine, item);
        go_north(machine);
        go_east(machine);
        go_north(machine);

    } else if item == "tambourine" {
        go_south(machine);
        go_south(machine);
        go_west(machine);
        go_north(machine);
        go_north(machine);
        take_item(machine, item);
        go_south(machine);
        go_south(machine);
        go_east(machine);
        go_north(machine);
        go_north(machine);
    } else if item == "fixed point" {
        go_south(machine); go_south(machine); go_south(machine);
        take_item(machine, item);
        go_north(machine); go_north(machine); go_north(machine);
    } else if item == "festive hat" {
        go_south(machine); go_south(machine); go_south(machine); go_south(machine);
        take_item(machine, item);
        go_north(machine); go_north(machine); go_north(machine); go_north(machine);
    } else if item == "jam" {
        go_south(machine); go_south(machine); go_south(machine); go_south(machine); go_west(machine); go_west(machine);
        take_item(machine, item);
        go_east(machine); go_east(machine); go_north(machine); go_north(machine); go_north(machine); go_north(machine); 
    } else if item == "easter egg" {
        go_south(machine); go_south(machine); go_south(machine); go_south(machine); go_west(machine); go_west(machine); go_south(machine);
        take_item(machine, item);
        go_north(machine); go_east(machine); go_east(machine); go_north(machine); go_north(machine); go_north(machine); go_north(machine); 
    } else if item == "asterisk" {
        go_south(machine); go_south(machine); go_south(machine); go_west(machine);
        take_item(machine, item);
         go_east(machine); go_north(machine); go_north(machine); go_north(machine);

    } else {
        // no op 
    }
}

/*
checkpoint
SWWWWW 

SWWW space heater    
SWS antenna
SSWNN tambourine
SSS fixed point
SSSS festive hat
SSSSWW jam
SSSSWWS easter egg

bad items

S infinite loop
SW escape pod
SS photons
SSWN molten lava
SSSSE magnet

good items

S  FF E
HATPHJE
XXXXXXX lighter
 X XXXX heavier
 XXXXXX lighter
  XXXXX lighter
 XXXXX  lighter
 XXXX   lighter
 XXX    lighter
 XX     lighter
  X     heavier
  XX    heavier
  XXX   heavier
  XXXX  lighter
  XXXXX lighter
  XXX X heavier
 XXXX X lighter
X XXX X lighter
X X X X heavier


*/