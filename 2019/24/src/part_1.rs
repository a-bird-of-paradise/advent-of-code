use std::collections::HashSet;
use crate::{point_to_int, int_to_point};

pub fn rating(grid: &Vec<char>) -> usize {
    let mut answer = 0;
    for i in 0..25 {
        if grid[i] == '#' {
            answer |= 1 << i;
        }
    }
    answer
}

pub fn repeated_state(grid: &Vec<char>) -> Vec<char> {
    let mut grid = grid.clone();

    let mut seen = HashSet::<Vec<char>>::new();
    let mut prev = grid.clone();
    seen.insert(prev.clone());

    grid = evolve(&prev);

    while !seen.contains(&grid) {
        seen.insert(grid.clone());
        prev = grid;
        grid = evolve(&prev);
    }
    grid
}

pub fn init_grid(raw_file : &str) -> Vec<char> {
    let file_rows : Vec<_> = raw_file
        .split('\n')
        .map(|line| line.strip_suffix('\r').unwrap_or(line))
        .collect();

    let mut grid = vec![' ';25];

    for row in 0..5usize {
        for col in 0..5usize {
            grid[point_to_int(col, row)] = file_rows[row].chars().nth(col).unwrap();
        }
    }
    grid
}

fn neighbours(i: usize) -> Vec<usize> {
    let mut answer = Vec::<usize>::new();

    let (x,y) = int_to_point(i);

    let col_start = if x == 0 { 0 } else { x-1 };
    let col_end = if x == 4 { 4 } else { x+1 };
    let row_start = if y == 0 { 0 } else { y-1 };
    let row_end = if y == 4 { 4 } else { y+1 };

    for row in row_start..=row_end {
        if int_to_point(i) == (x,row) {
            continue;
        }
        answer.push(point_to_int(x, row));
    }    
    for col in col_start..=col_end {
        if int_to_point(i) == (col,y) {
            continue;
        }
        answer.push(point_to_int(col, y));
    }
    answer.sort();
    answer
}

fn evolve(grid: &Vec<char>) -> Vec<char> {
    let mut answer = vec![' ';25];
    for row in 0..5usize {
        for col in 0..5usize {

            let mut alive_neighbours = 0;

            for point in neighbours(point_to_int(col, row)) {
                if grid[point] == '#' {
                    alive_neighbours += 1;
                }
            }

            if grid[point_to_int(col, row)] == '#' {
                if alive_neighbours == 1 {
                    answer[point_to_int(col, row)] = '#' ;
                } else {
                    answer[point_to_int(col, row)] = '.';
                }
            } else {
                if alive_neighbours == 1 || alive_neighbours == 2 {
                    answer[point_to_int(col, row)] = '#' ;
                } else {
                    answer[point_to_int(col, row)] = '.';
                }
            }
        }
    }
    answer
}

fn print_grid<T:std::fmt::Display> (grid: &Vec<T>){
    for row in 0..5 {
        for col in 0..5 {
            print!("{}",grid[point_to_int(col,row)]);
        }
        println!("");
    }
    println!("");
}

#[test]
fn test_rating() {
    let initial_state = init_grid("....#
#..#.
#..##
..#..
#....");
    let grid = repeated_state(&initial_state);
    assert_eq!(rating(&grid),2129920);
}

#[test]
fn test_repeated_state() {
    let initial_state = init_grid("....#
#..#.
#..##
..#..
#....");
    let grid = repeated_state(&initial_state);
    let answer: Vec<_> = "...............#.....#...".chars().collect();
    assert_eq!(grid,answer);
}

#[test]
fn test_neighbours() {
    let nbors = neighbours(0);
    let answer = vec![point_to_int(1, 0), point_to_int(0, 1)];
    assert_eq!(nbors,answer);

    let nbors = neighbours(point_to_int(2, 3));
    let mut answer = vec![point_to_int(2, 2), point_to_int(2, 4), point_to_int(1, 3), point_to_int(3, 3)];
    answer.sort();    
    assert_eq!(nbors,answer);
}

#[test]
fn test_evolve() {
    let initial_state : Vec<char> = "....##..#.#..##..#..#....".chars().collect();

    let state_1 = evolve(&initial_state);
    let state_2 = evolve(&state_1);
    let state_3 = evolve(&state_2);
    let state_4 = evolve(&state_3);

    let answer_1 : Vec<char> = "#..#.####.###.###.##.##..".chars().collect();
    let answer_2 : Vec<char> = "#####....#....#...#.#.###".chars().collect();
    let answer_3 : Vec<char> = "#....####....###.##..##.#".chars().collect();
    let answer_4 : Vec<char> = "####.....###..#.....##...".chars().collect();

    print_grid(&initial_state);
    print_grid(&state_1);
    print_grid(&answer_1);

    assert_eq!(state_1,answer_1);
    assert_eq!(state_2,answer_2);
    assert_eq!(state_3,answer_3);
    assert_eq!(state_4,answer_4);
}
