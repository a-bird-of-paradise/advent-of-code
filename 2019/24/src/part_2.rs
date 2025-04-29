use std::{collections::HashSet, ops::{Add, Mul}};

use crate::point_to_int;
use crate::part_1::init_grid;

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct GridLayer {
    grid: Vec<char>
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum GridDirection
{
    LowerLayer,
    ThisLayer,
    UpperLayer
}
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Row { pub row:usize }
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Column { pub col:usize }
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Layer { pub layer:i64 }
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Position { pub row:Row, pub col:Column }
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Location { pub pos:Position, pub layer:Layer }
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct RelativeLocation { pos:Position, layer:GridDirection }

impl Row { pub fn new(i: usize) -> Self { Self { row:i } } }
impl Column { pub fn new(i: usize) -> Self { Self { col:i } } }
impl Layer { pub fn new(i: i64) -> Self { Self { layer:i } } }
impl Position { pub fn new(row:Row, col: Column) -> Self { Self { row:row, col:col } } }
impl Location { pub fn new(pos:Position, layer:Layer) -> Self { Self { pos:pos, layer:layer} } }
impl RelativeLocation { pub fn new(pos:Position, layer:GridDirection) -> Self { Self { pos:pos, layer:layer} } }

impl Mul<usize> for Row {
    type Output = usize;
    fn mul(self, rhs: usize) -> usize {
        self.row * rhs
    }
}

impl Mul<Row> for usize {
    type Output = usize;
    fn mul(self, rhs: Row) -> usize {
        self * rhs.row
    }
}

impl Add<usize> for Column {
    type Output = usize;
    fn add(self, rhs: usize) -> usize {
        self.col + rhs
    }
}

impl Add<Column> for usize {
    type Output = usize;
    fn add(self, rhs: Column) -> usize {
        self + rhs.col
    }
}

impl GridLayer {
    pub fn new(grid: &Vec<char>) -> Self {
        Self { grid: grid.clone()  }
    }
}

pub fn index_to_position(index: usize) -> Position {
    Position::new(Row::new(index / 5 ), Column::new(index % 5))
}

pub fn position_to_index(pos: Position) -> usize {
   5 * pos.row + pos.col
}

fn right(pos: &Position) -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(pos.row.row),Column::new(pos.col.col+1)),GridDirection::ThisLayer)
}
fn left(pos: &Position) -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(pos.row.row),Column::new(pos.col.col-1)),GridDirection::ThisLayer)
}
fn up(pos: &Position) -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(pos.row.row-1),Column::new(pos.col.col)),GridDirection::ThisLayer)
}
fn down(pos: &Position) -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(pos.row.row+1),Column::new(pos.col.col)),GridDirection::ThisLayer)
}
fn right_out() -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(2),Column::new(3)),GridDirection::UpperLayer)
}
fn left_out() -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(2),Column::new(1)),GridDirection::UpperLayer)
}
fn up_out() -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(1),Column::new(2)),GridDirection::UpperLayer)
}
fn down_out() -> RelativeLocation {
    RelativeLocation::new(Position::new(Row::new(3),Column::new(2)),GridDirection::UpperLayer)
}
fn right_in() -> Vec<RelativeLocation> {
    vec![
        RelativeLocation{ pos: Position { row: Row { row: 0 }, col: Column { col: 0 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 1 }, col: Column { col: 0 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 2 }, col: Column { col: 0 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 3 }, col: Column { col: 0 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 4 }, col: Column { col: 0 } } , layer: GridDirection::LowerLayer }
    ]
}
fn left_in() -> Vec<RelativeLocation> {
    vec![
        RelativeLocation{ pos: Position { row: Row { row: 0 }, col: Column { col: 4 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 1 }, col: Column { col: 4 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 2 }, col: Column { col: 4 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 3 }, col: Column { col: 4 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 4 }, col: Column { col: 4 } } , layer: GridDirection::LowerLayer }
    ]
}
fn up_in() -> Vec<RelativeLocation> {
    vec![
        RelativeLocation{ pos: Position { row: Row { row: 4 }, col: Column { col: 0 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 4 }, col: Column { col: 1 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 4 }, col: Column { col: 2 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 4 }, col: Column { col: 3 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 4 }, col: Column { col: 4 } } , layer: GridDirection::LowerLayer }
    ]
}

fn down_in() -> Vec<RelativeLocation> {
    vec![
        RelativeLocation{ pos: Position { row: Row { row: 0 }, col: Column { col: 0 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 0 }, col: Column { col: 1 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 0 }, col: Column { col: 2 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 0 }, col: Column { col: 3 } } , layer: GridDirection::LowerLayer },
        RelativeLocation{ pos: Position { row: Row { row: 0 }, col: Column { col: 4 } } , layer: GridDirection::LowerLayer }
    ]
}

impl Position {
    pub fn neighbours(&self) -> Vec<RelativeLocation> {
        let mut answer = Vec::<RelativeLocation>::new();
        if ! (self.row.row == 2 && self.col.col == 2) {
            // up
            if self.row.row == 0 {
                answer.push(up_out());
            } else if self.row.row == 3 && self.col.col == 2 {
                answer.append(&mut up_in());
            } else {
                answer.push(up(self));
            }
            //down
            if self.row.row == 4 {
                answer.push(down_out());
            } else if self.row.row == 1 && self.col.col == 2 {
                answer.append(&mut down_in());
            } else {
                answer.push(down(self));
            }
            //left
            if self.col.col == 0 {
                answer.push(left_out());
            } else if self.row.row == 2 && self.col.col == 3 {
                answer.append(&mut left_in());
            } else {
                answer.push(left(self));
            }
            //right
            if self.col.col == 4 {
                answer.push(right_out());
            } else if self.row.row == 2 && self.col.col == 1 {
                answer.append(&mut right_in());
            } else {
                answer.push(right(self));
            }
        }
        answer
    }
}

impl Location {
    pub fn neighbours(&self) -> Vec<Location> {
        let rellocs = self.pos.neighbours();
        let mut answer = Vec::<Location>::new();

        for rel in rellocs {
            answer.push(Location 
                { pos: rel.pos, 
                    layer: Layer 
                    { layer: self.layer.layer +
                        match rel.layer {
                            GridDirection::LowerLayer => 1,
                            GridDirection::ThisLayer => 0,
                            GridDirection::UpperLayer => -1
                        }
                    }
                }
            )
        }
        answer
    }
}

pub fn evolve(currently_alive: &HashSet<Location>) -> HashSet<Location> {
    
    let mut answer = HashSet::<Location>::new();

    let mut empty_neighbours = HashSet::<Location>::new();

    for alive in currently_alive {
        for neighbour in alive.neighbours() {
            empty_neighbours.insert(neighbour);
        }
    }

    for alive in currently_alive {
        empty_neighbours.remove(alive);
    }

    // currently alive cells: if exactly one alive neighbour, remain alive 

    for alive in currently_alive {
        let ns = alive.neighbours();
        let mut alive_n = 0;
        for n in &ns {
            if currently_alive.contains(&n) {
                alive_n += 1;
            }
        }
        if alive_n == 1 {
            answer.insert(*alive);
        }
    }

    // currently dead cells: if one or two alive neighbours, become alive

    for dead in &empty_neighbours {
        let ns = dead.neighbours();
        let mut alive_n = 0;
        for n in &ns {
            if currently_alive.contains(&n) {
                alive_n += 1;
            }
        }
        if alive_n == 1 || alive_n == 2 {
            answer.insert(*dead);
        }

    }
    answer
}

fn print_alive(currently_alive: &HashSet<Location>) {
    for layer in -5..=5 {
        println!("Layer {layer}:");
        for row in 0..5 {
            for col in 0..5 {
                if currently_alive.contains(
                    &Location { pos: Position { row: Row { row: row} , col: Column { col: col } }, layer: Layer { layer: layer } }
                ) {
                    print!("#");
                } else {
                    print!(".");
                }
            }
            println!("");
        }
        println!("");
    }
}

#[test]
fn test_evolve() {
    let grid = init_grid("....#
#..#.
#..##
..#..
#....");

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

    for i in 1..=10 {
        currently_alive = evolve(&currently_alive);
    }

    assert_eq!(currently_alive.len(),99);
    
}

#[test] 
fn test_neighbours_01() {
    let cell_19 = Position { row: Row { row: 3 }, col: Column { col: 3 } };
    let mut cell_19_neighbours = cell_19.neighbours();
    cell_19_neighbours.sort();
    let mut answer = vec![
        RelativeLocation { pos:  Position { row: Row { row: 2 }, col: Column { col: 3 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 4 }, col: Column { col: 3 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 3 }, col: Column { col: 2 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 3 }, col: Column { col: 4 } }, layer: GridDirection::ThisLayer },
    ];
    answer.sort();
    assert_eq!(answer,cell_19_neighbours);
}
#[test] 
fn test_neighbours_02() {
    let cell_g = Position { row: Row { row: 1 }, col: Column { col: 1 } };
    let mut cell_g_neighbours = cell_g.neighbours();
    cell_g_neighbours.sort();
    let mut answer = vec![
        RelativeLocation { pos:  Position { row: Row { row: 0 }, col: Column { col: 1 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 2 }, col: Column { col: 1 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 0 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 2 } }, layer: GridDirection::ThisLayer },
    ];
    answer.sort();
    assert_eq!(answer,cell_g_neighbours);
}

#[test] 
fn test_neighbours_03() {
    let cell_d = Position { row: Row { row: 0 }, col: Column { col: 3 } };
    let mut cell_d_neighbours = cell_d.neighbours();
    cell_d_neighbours.sort();
    let mut answer = vec![
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 2 } }, layer: GridDirection::UpperLayer },
        RelativeLocation { pos:  Position { row: Row { row: 0 }, col: Column { col: 2 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 0 }, col: Column { col: 4 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 3 } }, layer: GridDirection::ThisLayer },
    ];
    answer.sort();
    assert_eq!(answer,cell_d_neighbours);
}

#[test] 
fn test_neighbours_04() {
    let cell_e = Position { row: Row { row: 0 }, col: Column { col: 4 } };
    let mut cell_e_neighbours = cell_e.neighbours();
    cell_e_neighbours.sort();
    let mut answer = vec![
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 2 } }, layer: GridDirection::UpperLayer },
        RelativeLocation { pos:  Position { row: Row { row: 2 }, col: Column { col: 3 } }, layer: GridDirection::UpperLayer },
        RelativeLocation { pos:  Position { row: Row { row: 0 }, col: Column { col: 3 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 4 } }, layer: GridDirection::ThisLayer },
    ];
    answer.sort();
    assert_eq!(answer,cell_e_neighbours);
}

#[test] 
fn test_neighbours_05() {
    let cell_14 = Position { row: Row { row: 2 }, col: Column { col: 3 } };
    let mut cell_14_neighbours = cell_14.neighbours();
    cell_14_neighbours.sort();
    let mut answer = vec![
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 3 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 3 }, col: Column { col: 3 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 2 }, col: Column { col: 4 } }, layer: GridDirection::ThisLayer },
        RelativeLocation { pos:  Position { row: Row { row: 0 }, col: Column { col: 4 } }, layer: GridDirection::LowerLayer },
        RelativeLocation { pos:  Position { row: Row { row: 1 }, col: Column { col: 4 } }, layer: GridDirection::LowerLayer },
        RelativeLocation { pos:  Position { row: Row { row: 2 }, col: Column { col: 4 } }, layer: GridDirection::LowerLayer },
        RelativeLocation { pos:  Position { row: Row { row: 3 }, col: Column { col: 4 } }, layer: GridDirection::LowerLayer },
        RelativeLocation { pos:  Position { row: Row { row: 4 }, col: Column { col: 4 } }, layer: GridDirection::LowerLayer },
    ];
    answer.sort();
    assert_eq!(answer,cell_14_neighbours);
}
