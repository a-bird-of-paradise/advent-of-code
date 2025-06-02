use std::collections::HashSet;

use aoc2024::AOC;
use aoc2024::{Point,Direction};
use itertools::Itertools;
pub struct Day06 {}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Default, Hash)]
struct GuardState {
    loc: Point,
    dir: Direction
}

fn rot_right(dir: &Direction) -> Direction {
    Direction { x: -dir.y, y: dir.x }
}

fn parse(input: &str) -> (HashSet<Point>, GuardState) {
    let mut obstacles = HashSet::new();
    let mut guard_loc = Point::new();
    let guard_dir = Direction{x: 0, y: -1};

    let mut row: i64 = 0;
    let mut col: i64 = 0;

    for c in input.chars() {
        match c {
            '.' =>                                              col += 1,
            '#' => { obstacles.insert(Point{x: col, y: row });  col += 1 },
            '^' => { guard_loc.x = col; guard_loc.y = row;      col += 1 },
            '\r' => {},
            '\n' => { row += 1; col = 0 },
            _ => panic!("Unexpected character in input")
        }
    }

    (obstacles,GuardState{loc:guard_loc, dir:guard_dir})
}

fn loops(obstacles: &HashSet<Point>, guard_state: &GuardState, max_x: i64, max_y: i64, visited_states: &HashSet<GuardState>) -> bool {
    let mut visited: HashSet<GuardState> = HashSet::new();

    let mut loops = false;

    let mut gs: GuardState = *guard_state;

    loop {
        visited.insert(gs);

        while obstacles.contains(&(gs.loc + gs.dir)) {
            gs.dir = rot_right(&gs.dir);
        }
        gs.loc += gs.dir;

        if gs.loc.x > max_x || gs.loc.y > max_y || gs.loc.x < 0 || gs.loc.y < 0 {
            break;
        }

        if  visited_states.contains(&gs) || visited.contains(&gs) {
            loops = true;
            break;
        }
    }
    loops
}

impl AOC for Day06 {
    fn part_one(&self, input: &str) -> String {   
        let (obstacles, mut guard_state) = parse(input);
        let mut visited: HashSet<Point> = HashSet::new();

        let max_x = obstacles.iter().map(|p| p.x).max().unwrap();
        let max_y = obstacles.iter().map(|p| p.y).max().unwrap();
        
        loop {            
            visited.insert(guard_state.loc);

            if obstacles.contains(&(guard_state.loc + guard_state.dir)) {
                guard_state.dir = rot_right(&guard_state.dir);
            }
            guard_state.loc += guard_state.dir;

            if guard_state.loc.x > max_x || guard_state.loc.y > max_y || guard_state.loc.x < 0 || guard_state.loc.y < 0 {
                break;
            }
        }

        visited.len().to_string()
    }

    fn part_two(&self, input: &str) -> String {
        let (mut obstacles, guard_state) = parse(input);

        let mut visited: HashSet<GuardState> = HashSet::new();
        let mut visited_points: HashSet<Point> = HashSet::new();

        let max_x = obstacles.iter().map(|p| p.x).max().unwrap();
        let max_y = obstacles.iter().map(|p| p.y).max().unwrap();

        let mut gs = guard_state.clone();

        let mut answer = 0;
        
        loop {

            visited.insert(gs);
            visited_points.insert(gs.loc);

            while obstacles.contains(&(gs.loc + gs.dir)) {
                gs.dir = rot_right(&gs.dir);
            } 
            
            {
                // no block ahead
                // put one in; does it loop? 
    
                let next_gs = GuardState{ loc: gs.loc + gs.dir, dir: gs.dir };
                if next_gs.loc.x >= 0 
                    && next_gs.loc.y >= 0 
                    && next_gs.loc.x <= max_x 
                    && next_gs.loc.y <= max_y 
                    && next_gs.loc != guard_state.loc 
                    && !visited_points.contains(&next_gs.loc){
                    obstacles.insert(next_gs.loc);
                    if loops(&obstacles,&gs, max_x, max_y, &visited) {
                        answer += 1;
                    }
                    obstacles.remove(&next_gs.loc);
                }
            }
            gs.loc += gs.dir;

            if gs.loc.x > max_x || gs.loc.y > max_y || gs.loc.x < 0 || gs.loc.y < 0 {
                break;
            }
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
        let test_file_path = "test-inputs/day06.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day06 {};
        assert_eq!(my_struct.part_one(&file),"41")
    }
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day06.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day06 {};
        assert_eq!(my_struct.part_two(&file),"6");
    }

    #[test]
    fn test_rot_right() {
        let up = Direction{x:0, y: -1};
        let down = Direction{x:0,y:1};
        let right = Direction{x:1,y:0};
        let left = Direction{x:-1,y:0};

        assert_eq!(right,rot_right(&up));
        assert_eq!(down,rot_right(&right));
        assert_eq!(left,rot_right(&down));
        assert_eq!(up,rot_right(&left));
    }
}