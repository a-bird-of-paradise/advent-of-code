use std::{cmp::Ordering, collections::{BTreeMap,BTreeSet}};

use aoc2024::AOC;
pub struct Day05 {}

use nom::{
  bytes::complete::tag, 
  character::complete::{i64, line_ending}, 
  multi::{many1, separated_list1}, 
  sequence::{separated_pair,terminated,pair}, 
  IResult, 
  Parser,
  combinator::opt
};

use itertools::Itertools;

fn read_pair(input: &str) -> IResult<&str,(i64,i64)> {
    separated_pair(i64, tag("|"), i64).parse(input)
}

fn read_part_1_line(input: &str) -> IResult<&str,(i64,i64)> {
    terminated(read_pair, line_ending).parse(input)
}

fn read_part_1_lines(input: &str) -> IResult<&str,Vec<(i64,i64)>> {
    many1(read_part_1_line).parse(input)
}

fn read_part_1(input: &str) -> IResult<&str,Vec<(i64,i64)>> {
    terminated(read_part_1_lines, line_ending).parse(input)
}

fn read_part_2_line(input: &str) -> IResult<&str,Vec<i64>> {
    terminated(separated_list1(tag(","), i64), opt(line_ending)).parse(input)
}

fn read_part_2_lines(input: &str) -> IResult<&str,Vec<Vec<i64>>> {
    many1(read_part_2_line).parse(input)
}

fn read_input(input: &str) -> IResult<&str, ( Vec<(i64,i64)>, Vec<Vec<i64>> )> {
    pair(read_part_1,read_part_2_lines).parse(input)
}

fn make_ordering_rules(rules: &[(i64,i64)]) -> BTreeMap<i64,BTreeSet<i64>> {
    let mut answer: BTreeMap<i64, BTreeSet<i64>> = BTreeMap::new();

    for (k,v) in rules {
        answer.entry(*k).or_default().insert(*v);
    }

    answer
}

fn is_ordered(list: &[i64], rules: &BTreeMap<i64,BTreeSet<i64>>) -> bool {
    let mut answer = true;

    'outer: for i in 0..list.len() -1 {
        if !rules.contains_key(&list[i]) {
            println!("!!{:?}",list[i]);   
            answer = false; 
            break;
        }
        for j in i+1..list.len() {
            if !rules.get(&list[i]).unwrap().contains(&list[j]) {
                answer = false;
                break 'outer;
            }
        }
    }
    answer
}

impl AOC for Day05 {
    fn part_one(&self, input: &str) -> String {   

        let (pre_rules,lists) = read_input(input).unwrap().1;
        let rules = make_ordering_rules(&pre_rules);

        let mut answer: i64 = 0;

        for list in lists {
            if is_ordered(&list, &rules) {
                answer += &list[&list.len()/2];
            }
        }

        answer.to_string()
    }

    fn part_two(&self, input: &str) -> String {

        let (pre_rules,mut lists) = read_input(input).unwrap().1;
        let rules = make_ordering_rules(&pre_rules);

        let comparer = | left: &i64, right: &i64 | {
            let (left,right) =(*left, *right);
            if pre_rules.contains(&(left,right)) {
                Ordering::Less
            } else if pre_rules.contains(&(right,left)) {
                Ordering::Greater
            } else {
                Ordering::Equal
            }
        };

        lists
            .iter_mut()
            .map(|line| {
                if is_ordered(&line, &rules) {
                    0 as i64
                } else {
                    line.sort_by(comparer);
                    line[line.len()/2]
                }
            })
            .sum::<i64>()
            .to_string()
    }
}

#[allow(dead_code)]
fn part_two_slow(input: &str) -> String {

    let (pre_rules,lists) = read_input(input).unwrap().1;
    let rules = make_ordering_rules(&pre_rules);

    let mut answer: i64 = 0;

    for list in &lists {
        println!("{:?}",list);
        if !is_ordered(&list, &rules) {
            for perm_pre in list.into_iter().permutations(list.len()) {
                let perm: Vec<i64> = perm_pre.into_iter().map(|x| *x).collect(); 
                if is_ordered(&perm, &rules) {
                    answer += perm[perm.len()/2];
                    break;
                }
            }
        }
    }

    answer.to_string()
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day05.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day05 {};
        assert_eq!(my_struct.part_one(&file),"143")
    }

    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day05.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day05 {};
        assert_eq!(my_struct.part_two(&file),"123");
    }

    #[test]
    fn part_2_slow() {
        let test_file_path = "test-inputs/day05.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        assert_eq!(part_two_slow(&file),"123");
    }
}