use aoc2024::AOC;
use aoc2024::Point;
use std::collections::BTreeMap;
use itertools::Itertools;

pub struct Day08 {} 

fn get_max_y(input: &str) -> i64 {
    input.lines().count() as i64 - 1
}

fn get_max_x(input: &str) -> i64 {
    input.lines().map(|x| x.len()).max().unwrap() as i64 - 1
}

fn read_input(input: &str) -> BTreeMap<char, Vec<Point>> {
    let mut row = 0;
    let mut col = 0;
    let mut answer: BTreeMap<char, Vec<Point>> = BTreeMap::new();

    for c in input.chars() {
        match c {
            '.'                               => { col += 1 },
            '\r'                              => {},
            '\n'                              => { col = 0; row += 1 },
            'a'..='z' | 'A'..='Z' | '0'..='9' => { answer.entry(c).or_default().push(Point{x:col, y: row}); col += 1 },
            _                                 => { println!("Unexpected character: {c} at x: {col}, y: {row} "); col += 1 }
        }
    }
    answer
}

fn nodes(input: &BTreeMap<char, Vec<Point>>) -> Vec<Point> {
    let mut answer = Vec::<Point>::new();
    for (_, v) in input {
        for p in v.into_iter().permutations(2) {
            let diff = *p[1]-*p[0];
            let next_point = *p[1] + diff; 
            answer.push(next_point);
        }
    }
    answer
}

fn nodes_2(input: &BTreeMap<char, Vec<Point>>, max_x: i64, max_y: i64) -> Vec<Point> {
    let mut answer = Vec::<Point>::new();
    for (_, v) in input {
        for p in v.into_iter().permutations(2) {
            let diff = *p[1]-*p[0];
            let mut next_point = *p[1];
            while next_point.x >= 0 && next_point.y >= 0 && next_point.x <= max_x && next_point.y <= max_y {
                answer.push(next_point);
                next_point += diff;
            }
        }
    }
    answer
}

impl AOC for Day08 {
    fn part_one(&self, input: &str) -> String {
        let input_map = read_input(input);
        let max_x = get_max_x(input);
        let max_y = get_max_y(input);
        let nodes = nodes(&input_map);

        nodes
            .into_iter()
            .filter(|p| p.x >= 0 && p.y >= 0 && p.x <= max_x && p.y <= max_y)
            .unique()
            .count()
            .to_string()
    }

    fn part_two(&self, input: &str) -> String {  
        let input_map = read_input(input);
        let max_x = get_max_x(input);
        let max_y = get_max_y(input);
        let nodes = nodes_2(&input_map, max_x, max_y);

        nodes
            .into_iter()
            .filter(|p| p.x >= 0 && p.y >= 0 && p.x <= max_x && p.y <= max_y)
            .unique()
            .count()
            .to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day08.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day08 {};
        assert_eq!(my_struct.part_one(&file),"14")
    } 
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day08.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day08 {};
        assert_eq!(my_struct.part_two(&file),"34");
    }
}