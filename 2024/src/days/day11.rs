use std::collections::BTreeMap;

use aoc2024::{AOC,Tree};
pub struct Day11;

use nom::{
    character::complete::{space1, u64}, 
    combinator::opt, 
    multi::many1, 
    sequence::terminated, 
    IResult, 
    Parser
};

fn read_u64(input: &str)->IResult<&str,u64> { u64.parse(input) }

fn read_input(input: &str) -> Vec<u64> {
    many1(terminated(read_u64,opt(space1))).parse(input).unwrap().1
}

fn process_stone(stone: u64) -> (u64,Option<u64>)
{
    if stone == 0 {
        (1, None)
    } else if (stone.checked_ilog10().unwrap_or(0)+1) % 2 == 0 {
        let n_digits = stone.checked_ilog10().unwrap_or(0)+1;
        let divisor = 10_u64.pow(n_digits / 2);
        (stone / divisor, Some(stone % divisor))
    } else {
        (2024 * stone, None)
    }
}

impl AOC for Day11 {
    fn part_one(&self, input: &str) -> String {
        let stones = read_input(input);
        let mut tree = Tree::<u64>::new(&stones);

        for _i in 1..=25 { tree.root.as_mut().unwrap().walk(); }

        tree.num_leaves().to_string()
    }

    fn part_two(&self, input: &str) -> String {
         let stones = read_input(input);

        let mut cache = BTreeMap::<u64, Vec<u64>>::new();
        let mut stone_count = BTreeMap::<u64,u64>::new();
        let mut new_stone_count = stone_count.clone();

        let init: Vec<_> = stones
            .iter()
            .map(|s| process_stone(*s))
            .collect();

        for (stone, result) in stones.iter().zip(init.iter()) {
            cache.entry(*stone).or_default().push(result.0);
            if let Some(r) = result.1 {
                cache.entry(*stone).or_default().push(r);
            }
        }

        for s in stones { stone_count.entry(s).or_insert(1); }

        for _i in 1..=75 {
            new_stone_count.clear();
            for (stone, count) in stone_count {
                if !cache.contains_key(&stone) {
                    let result = process_stone(stone);
                    cache.entry(stone).or_default().push(result.0);
                    if let Some(r) = result.1 { cache.entry(stone).and_modify(|e| e.push(r)); }
                }

                let ck = &cache[&stone];

                for new_stone in ck {
                    *new_stone_count.entry(*new_stone).or_default() += count;
                }
            }
            stone_count = new_stone_count.clone();
        }

        stone_count
            .iter()
            .map(|(_,count)| count)
            .sum::<u64>()
            .to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day11.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day11 {};
        assert_eq!(my_struct.part_one(&file),"55312")
    } 
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day11.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day11 {};
        assert_eq!(my_struct.part_two(&file),"65601038650482");
    }
}