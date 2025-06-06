use aoc2024::AOC;
use std::collections::BTreeMap;
pub struct Day09 {} 

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
enum Block {
    Empty,
    Number(i64)
}

fn read_input(input: &str) -> Vec<Block> {
    let mut is_even = true; // start position 0
    let mut answer = Vec::<Block>::new();
    let mut file_id: i64 = 0;
    let _ = input
        .chars()
        .into_iter()
        .map(|c| {
        
            match c {
                '0'..='9' => {
                    let mut d = ( (c as u32) - '0' as u32) as i64;
                    if is_even {
                        while d > 0 {
                            answer.push(Block::Number(file_id));
                            d -= 1;
                        }
                        file_id += 1;
                    } else {
                        while d > 0 {
                            answer.push(Block::Empty);
                            d -= 1;
                        }
                    }
                    is_even = !is_even;
                },
                _ => { println!("Unexpected character: {c}"); }
            };

        c })
        .collect::<Vec<char>>();
    answer
}

#[allow(dead_code)]
fn diskdump(disk: &Vec<Block>) {
    disk
        .iter()
        .map(|b| match b {
            Block::Empty => { print!(".") },
            Block::Number(i) => { print!("{i}") }
        })
        .count();
    println!("");
}

impl AOC for Day09 {
    fn part_one(&self, input: &str) -> String {
        let mut disk = read_input(input);

        let mut first_unused_block: usize = disk.len() - 1;
        let mut last_used_block: usize = 0;

        for i in 0..disk.len() {
            match disk[i] {
                Block::Empty => { if i < first_unused_block { first_unused_block = i } },
                Block::Number(_) => { if i > last_used_block { last_used_block = i } } 
            }
        }

        while first_unused_block < last_used_block {
            disk[first_unused_block] = disk[last_used_block];
            disk[last_used_block] = Block::Empty;

            first_unused_block += 1;
            last_used_block -= 1; 

            while let Block::Number(_i) = disk[first_unused_block] { first_unused_block += 1 }
            while let Block::Empty = disk[last_used_block] { last_used_block -= 1 }
        }

        disk
            .iter()
            .enumerate()
            .map(|(i,b)| {
                match b {
                    Block::Empty => 0,
                    Block::Number(n) => n * ( i as i64)
                }
            })
            .sum::<i64>()
            .to_string()
    }

    fn part_two(&self, input: &str) -> String {  
        
        let mut disk = read_input(input);
        let mut first_blank_index = 0usize;

        let mut first_block_pointer: BTreeMap<i64,usize> = Default::default();
        let mut block_length: BTreeMap<i64,usize> = Default::default();

        let mut current_to_move = disk
            .iter()
            .enumerate()
            .map(|(i,b)| 
                match b { 
                    Block::Empty => {0}, 
                    Block::Number(n) => { 
                        first_block_pointer.entry(*n).or_insert(i);
                        *block_length.entry(*n).or_insert(0) += 1;
                        *n
                    } 
                }
            )
            .max()
            .unwrap();

        while current_to_move > 0 {
            //println!("!{current_to_move}");
            let mut pointer: usize = first_blank_index;

            while let Block::Number(_) = disk[pointer] { pointer+=1; }
            first_blank_index = pointer;

            let needed_length = block_length[&current_to_move];
            let ubound = first_block_pointer[&current_to_move];

            let mut all_empty = (pointer..pointer+needed_length)
                .map(|i| 
                    match disk[i] { 
                        Block::Empty => { true }, 
                        Block::Number(_i) => { false }
                    }
                )
                .reduce(|acc, e| { acc && e } )
                .unwrap();

            while !all_empty {
                pointer += 1;
                if pointer+needed_length >= disk.len() {
                    break;
                }
                if pointer >= ubound {
                    break;
                }
                all_empty = (pointer..pointer+needed_length)
                .map(|i| 
                    match disk[i] { 
                        Block::Empty => { true }, 
                        Block::Number(_i) => { false }
                    }
                )
                .reduce(|acc, e| { acc && e } )
                .unwrap() ;
            }

            if all_empty && pointer < ubound {
                for i in 0..needed_length {
                    disk[pointer+i] = Block::Number(current_to_move);
                    disk[first_block_pointer[&current_to_move]+i] = Block::Empty;
                }
            }
            //diskdump(&disk);
            current_to_move -= 1;
        }

        disk
            .iter()
            .enumerate()
            .map(|(i,b)| {
                match b {
                    Block::Empty => 0,
                    Block::Number(n) => n * ( i as i64)
                }
            })
            .sum::<i64>()
            .to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day09.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day09 {};
        assert_eq!(my_struct.part_one(&file),"1928")
    } 
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day09.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day09 {};
        assert_eq!(my_struct.part_two(&file),"2858");
    }
}