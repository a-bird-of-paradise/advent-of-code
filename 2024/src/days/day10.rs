use aoc2024::{AOC,Point};
use std::collections::HashSet;
pub struct Day10 {}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Default, Hash)]
struct Path(Vec<Point>);

fn read_input(input: &str) -> Vec<Vec<u8>> {
    let mut answer: Vec<Vec<u8>> = Default::default();
    let mut this_row: Vec<u8> = Default::default();
    
    for c in input.chars() {
        match c {
            '0'..='9' => { this_row.push(c as u8 - '0' as u8) },
            '\r' => {},
            '\n' => { answer.push(this_row.clone()); this_row.clear(); }
            _ => { println!("Unexpected character: {c}") }
        }
    }
    if !this_row.is_empty() { answer.push(this_row.clone()); }
    answer
}

fn scores(from: &Point, grid: &Vec<Vec<u8>>, current_level: u8) -> HashSet<Point> {

    let mut answer: HashSet<Point> = Default::default();

    // if not current_level then not a path, fail 

    if grid[from.y as usize][from.x as usize] != current_level { return answer }

    // success

    if grid[from.y as usize][from.x as usize] >= 9 { answer.insert(*from); return answer}

    let max_x = grid[0].len() as i64;
    let max_y = grid.len() as i64;

    if from.x > 0       { answer.extend(scores(&Point{x:from.x-1, y:from.y}, grid, current_level+1)); }
    if from.y < max_y-1 { answer.extend(scores(&Point{x:from.x, y:from.y+1}, grid, current_level+1)); }
    if from.x < max_x-1 { answer.extend(scores(&Point{x:from.x+1, y:from.y}, grid, current_level+1)); }
    if from.y > 0       { answer.extend(scores(&Point{x:from.x, y:from.y-1}, grid, current_level+1)); }

    answer
}

fn paths(current_path: &mut Path, grid: &Vec<Vec<u8>>, found_paths: &mut HashSet<Path>) {

    let from = *current_path.0.last().unwrap();
    let current_level = grid[from.y as usize][from.x as usize];

    if current_level == 9 {
        found_paths.insert(current_path.clone());
        return
    }

    let max_x = grid[0].len() as i64;
    let max_y = grid.len() as i64;

    if from.x > 0 && grid[from.y as usize][(from.x-1) as usize] == current_level + 1 { 
        current_path.0.push(Point{x:from.x-1,y:from.y});
        paths(current_path,grid,found_paths);
        current_path.0.pop();
    }

    if from.x < max_x - 1 && grid[from.y as usize][(from.x+1) as usize] == current_level + 1 { 
        current_path.0.push(Point{x:from.x+1,y:from.y});
        paths(current_path,grid,found_paths);
        current_path.0.pop();
    }

    if from.y > 0 && grid[(from.y-1) as usize][from.x as usize] == current_level + 1 { 
        current_path.0.push(Point{x:from.x,y:from.y-1});
        paths(current_path,grid,found_paths);
        current_path.0.pop();
    }

    if from.y < max_y - 1 && grid[(from.y+1) as usize][from.x as usize] == current_level + 1 { 
        current_path.0.push(Point{x:from.x,y:from.y+1});
        paths(current_path,grid,found_paths);
        current_path.0.pop();
    }

}

impl AOC for Day10 {
    fn part_one(&self, input: &str) -> String {
        let grid = read_input(input);

        let mut trailheads = Vec::<Point>::new();

        for (i, row) in grid.iter().enumerate() {
            for (j, u) in row.iter().enumerate() {
                if *u == 0 {
                    trailheads.push(Point{x:j as i64, y:i as i64});
                }
            }
        }

        let mut answer = 0;

        for trailhead in trailheads {
            answer += scores(&trailhead, &grid,0).len();
        }

        answer.to_string()
    }
    
    fn part_two(&self, input: &str) -> String {
        let grid = read_input(input);

        let mut trailheads = Vec::<Point>::new();

        for (i, row) in grid.iter().enumerate() {
            for (j, u) in row.iter().enumerate() {
                if *u == 0 {
                    trailheads.push(Point{x:j as i64, y:i as i64});
                }
            }
        }

        let mut answer = 0;

        for trailhead in trailheads {
            let mut found_paths: HashSet<Path> = Default::default();
            paths(&mut Path(vec![trailhead]),&grid,&mut found_paths);
            answer += found_paths.len();
        }

        answer.to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day10.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day10 {};
        assert_eq!(my_struct.part_one(&file),"36")
    } 
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day10.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day10 {};
        assert_eq!(my_struct.part_two(&file),"81");
    }
}