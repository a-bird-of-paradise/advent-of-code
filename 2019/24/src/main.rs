
use std::{collections::HashSet, env, fs};
use part_1::{init_grid,rating,repeated_state};
use part_2::{evolve, Column, Layer, Location, Position, Row};

mod part_1;
mod part_2;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(&file_path).expect("Should have been able to read the file");

    let mut grid = init_grid(&raw_file);

    grid = repeated_state(&grid);

    println!("{}",rating(&grid));

    // part 2 

    grid = init_grid(&raw_file);

    let mut currently_alive = HashSet::<Location>::new();

    for row in 0..5 {
        for col in 0..5 {
            if grid[point_to_int(col, row)] == '#' {
                currently_alive.insert(
                    Location { pos: Position { row: Row { row: row }, col: Column { col: col } }, layer: Layer { layer: 0 } }
                );
            }
        }
    }

    for i in 1..=200 {
        currently_alive = evolve(&currently_alive);
    }

   println!("{}",currently_alive.len());

}

fn point_to_int (col: usize, row: usize) -> usize { 5 * row + col }
fn int_to_point(i: usize) -> (usize,usize) { (i % 5, i / 5) }


#[test]
fn test_point_to_int() {
    assert_eq!(point_to_int(3, 4),5*4 + 3);
    assert_eq!(point_to_int(2, 1),7);
}

#[test]
fn test_int_to_point() {
    assert_eq!(int_to_point(0),(0,0));
    assert_eq!(int_to_point(1),(1,0));
    assert_eq!(int_to_point(7),(2,1));
}
