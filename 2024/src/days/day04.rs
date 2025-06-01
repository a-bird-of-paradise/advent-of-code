use aoc2024::{AOC,Point,Direction};
pub struct Day04 {}

use std::collections::{BTreeMap,BTreeSet};

fn make_index(input: &str) -> BTreeMap<char, BTreeSet<Point>> {
    let mut answer: BTreeMap<char,BTreeSet<Point>> = BTreeMap::new();

    let mut point: Point = Point { x : 0, y : 0 };
    let po_next: Direction = Direction { x: 1, y: 0 };

    for c in input.chars() {
        if c == '\r' {
            continue;
        } else if c == '\n' {
            point.x = 0;
            point.y += 1;
            continue;
        } else if !(c == 'X' || c == 'M' || c == 'A' || c == 'S') {
            point += po_next;
            continue;
        }

        answer.entry(c).or_default().insert(point);

        point += po_next;
    }
    answer
}

fn get_directions() -> BTreeSet<Direction> {
    let mut directions : BTreeSet<Direction> = BTreeSet::new();
    
    for dx in -1..=1 {
        for dy in -1..=1 {
            if dx == 0 && dy == 0 { continue; }
            directions.insert(Direction { x: dx, y: dy });
        }
    }
    directions
}

fn count_xmas(index: &BTreeMap<char,BTreeSet<Point>>) -> usize {
    let mut answer = 0;
    let directions = get_directions();

    for x in &index[&'X'] {
        for dir in &directions {
            if index[&'M'].contains(&(*x + *dir)) && index[&'A'].contains(&(*x + *dir * 2)) && index[&'S'].contains(&(*x + *dir * 3)) {
                answer += 1;
            }
        }

    }
    answer
}

fn loc_mas(index: &BTreeMap<char,BTreeSet<Point>>) -> BTreeMap<Point,BTreeSet<Direction>> {
    // returns a map, key = location of A in MAS and value = set of direction vectors 
    // but only allows for intercardinal dirs
    let mut answer: BTreeMap<Point,BTreeSet<Direction>> = BTreeMap::new();
    let directions = get_directions();

    for m in &index[&'M'] {
        for dir in &directions {
            if dir.is_cardinal() { continue; }
            if index[&'A'].contains(&(*m + *dir)) && index[&'S'].contains(&(*m + *dir * 2)) {
                answer.entry(*m + *dir).or_default().insert(*dir);
            }
        }

    }
    answer
}

fn count_intercardinal_pairs(mas: &BTreeMap<Point,BTreeSet<Direction>>) -> usize {
    let mut answer = 0;
    for (_, v)  in mas {
        if v.len() == 2 {
            answer += 1;
        }
    }
    answer
}

impl AOC for Day04 {
    fn part_one(&self, input: &str) -> String {       
        count_xmas(&make_index(input)).to_string()
    }

    fn part_two(&self, input: &str) -> String {     
        count_intercardinal_pairs(&loc_mas(&make_index(input))).to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day04.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day04 {};
        assert_eq!(my_struct.part_one(&file),"4")
    }

    #[test]
    fn part_1_2() {
        let test_file_path = "test-inputs/day04.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day04 {};
        assert_eq!(my_struct.part_one(&file),"18")
    }

    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day04.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day04 {};
        assert_eq!(my_struct.part_two(&file),"9");
    }
}