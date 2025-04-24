mod intcode;

use std::{env,fs, i64, thread};
use intcode::{Machine, MachineStatus};

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();
        
    thread::scope(|scope| {
        let t1 = scope.spawn(|| part_1(&numbers));
        let t2 = scope.spawn(|| part_2(&numbers));

        println!("{}",t1.join().unwrap());
        println!("{}",t2.join().unwrap());
    });
}

fn part_1(program: &Vec<i64>) -> i64 {

    let answer: i64;

    let mut machines = vec![Machine::new(&program);50];

    for i in 0..50 {
        machines[i].input.push(i as i64);
    }

    'outer: loop {
        for i in 0..50 {
            machines[i].tick_with_default_read(-1);
            if machines[i].output.len() == 3 {
                let dest = machines[i].output[0];
                let x = machines[i].output[1];
                let y = machines [i].output[2];

                if dest == 255 {
                    answer = y;
                    break 'outer;
                }

                machines[i].output.clear();

                if dest >= 0 && dest < 50 {
                    machines[dest as usize].input.push(x);
                    machines[dest as usize].input.push(y);
                }
            }
        }
    }
    answer
}

fn part_2(program: &Vec<i64>) -> i64 {

    let answer: i64;

    let mut machines = vec![Machine::new(&program);50];

    for i in 0..50 {
        machines[i].input.push(i as i64);
    }

    let mut nat_x : i64 = i64::MAX;
    let mut nat_y : i64 = i64::MAX;
    let mut nat_last_sent_x : i64 = i64::MIN;
    let mut nat_last_sent_y : i64 = i64::MIN;
    let mut last_printed_y : i64 = -12345;

    let mut idle_counter = 0;

    'outer: loop {

        for i in 0..50 {
            let status = machines[i].tick();

            if status == MachineStatus::ReadBlocked {
                machines[i].input.push(-1);
                machines[i].tick();
                idle_counter += 1;
            } 

            if machines[i].output.len() == 3 {
                idle_counter = 0;
                let dest = machines[i].output[0];
                let x = machines[i].output[1];
                let y = machines[i].output[2];

                machines[i].output.clear();

                if dest == 255 {
                    nat_x = x;
                    nat_y = y;
                } else if dest >= 0 && dest < 50 {
                    machines[dest as usize].input.push(x);
                    machines[dest as usize].input.push(y);
                }
            }

            if machines[i].output.len() != 0 || machines[i].input.len() != 0 {
                idle_counter = 0;
            }
        }

        if idle_counter > 200 { // every machine read -1 last time and no machine sent a packet (or is filling a packet?)
            if nat_last_sent_y == nat_y && nat_last_sent_x == nat_x && nat_y != i64::MAX {
                if last_printed_y == nat_y {
                    answer = nat_y;
                    break 'outer;
                } else {
                    last_printed_y = nat_y;
                }
            }
            machines[0].input.push(nat_x);
            machines[0].input.push(nat_y);
            nat_last_sent_y = nat_y;
            nat_last_sent_x = nat_x;
            idle_counter = 0;
        }
    }
    answer
}