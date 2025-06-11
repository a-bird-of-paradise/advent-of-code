use std::{collections::{HashMap, HashSet}, ops::{Deref, DerefMut}};

use aoc2024::{AOC, Point, Direction};
use nom::{character::complete::{line_ending, one_of}, combinator::opt, multi::{fold, many1}, sequence::terminated, IResult, Parser};

pub struct Day15;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
enum Move { Up, Down, Left, Right }

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
enum Block { Wall, Robot, Empty, Box, BoxLeft, BoxRight }

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct Grid<T> {
    pub cells: Vec<T>,
    pub width: usize,
    pub height: usize
}

impl<T> Grid<T> {
    pub fn get(&self, x: usize, y: usize) -> &T {
        &self.cells[x + y * self.width]
    }

    pub fn set(&mut self, x: usize, y: usize) -> &mut T {
        &mut self.cells[x + y * self.width]
    }

    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.cells.iter()
    }

    pub fn index_to_point(&self, index: usize) -> Point {
        Point { x: index.rem_euclid(self.width as usize) as i64, y: index.div_euclid(self.width as usize) as i64 }
    }

    pub fn point_to_index(&self, point: &Point) -> usize {
        (point.y as usize * self.width) + point.x as usize
    }
}

impl<T> IntoIterator for Grid<T> {
    type Item = T;
    type IntoIter = std::vec::IntoIter<Self::Item>;

    fn into_iter(self) -> Self::IntoIter {
        self.cells.into_iter()
    }
}

impl<T> Deref for Grid<T> {
    type Target = [T]; 
    fn deref(&self) -> &[T] {
        &self.cells[..]
    }
}

impl<T> DerefMut for Grid<T> {
    fn deref_mut(&mut self) -> &mut [T] {
        &mut self.cells[..]
    }
}

impl<T> From<Vec<Vec<T>>> for Grid<T> where T:Clone {
    fn from(item: Vec<Vec<T>>) -> Self {
        let height = item.len();
        let width = item[0].len();
        let inner = item.iter().flatten().cloned().collect();
        Self { cells: inner, width: width, height: height }
    }
}

impl<T> std::fmt::Display for Grid<T> where T: std::fmt::Display {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        for y in 0..self.height {
            for x in 0..self.width {
                write!(f,"{}",self.get(x, y))?;
            }
            write!(f,"\n")?;
        }
        Ok(())
    }
}

impl std::fmt::Display for Block {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f,"{}",match self {
            Block::Wall     => { '#' },
            Block::Empty    => { '.' },
            Block::Box      => { 'O' },
            Block::Robot    => { '@' },
            Block::BoxRight => { ']' },
            Block::BoxLeft  => { '[' }
        })?;
        Ok(())
    }
}

impl From<char> for Move {
    fn from(item: char) -> Self {
        match item {
            'v' | 'V'   => { Self::Down                                 },
            '<'         => { Self::Left                                 },
            '>'         => { Self::Right                                },
            '^'         => { Self::Up                                   },
            _           => { panic!("Unrecognised character: {item}");  }
        }
    }
}

impl From<char> for Block {
    fn from(item: char) -> Self {
        match item {
            'o' | 'O'   => { Self::Box                                  },
            '.'         => { Self::Empty                                },
            '#'         => { Self::Wall                                 },
            '@'         => { Self::Robot                                },
            '['         => { Self::BoxLeft                              },
            ']'         => { Self::BoxRight                             },
            _           => { panic!("Unrecognised character: {item}");  }
        }
    }
}

impl From<Move> for Direction {
    fn from(item: Move) -> Self {
        match item {
            Move::Down  => { Self{x: 0, y: 1}   },
            Move::Up    => { Self{x: 0, y:-1}   },
            Move::Left  => { Self{x:-1, y: 0}   },
            Move::Right => { Self{x: 1, y: 0}   }
        }
    }
}

fn read_block(input: &str) -> IResult<&str, Block> {
    let (input, c) = one_of(".#O@[]").parse(input)?;
    let block = Block::from(c);
    Ok((input,block))
}

fn read_grid(input: &str) -> IResult<&str,Grid<Block>> {
    let (input, x) = many1(terminated(many1(read_block),line_ending)).parse(input)?;
    Ok((input,Grid::from(x)))
}

fn read_move(input: &str) -> IResult<&str,Move> {
    let (input, c) = one_of("<>^Vv").parse(input)?;
    Ok((input,c.into()))
}

fn read_moves(input: &str) -> IResult<&str,Vec<Move>> {
    let (input, z) = fold(1.., 
        terminated(many1(read_move), opt(line_ending)),
        Vec::new,
        |mut acc: Vec<_>, item| {
            acc.push(item);
            acc
            }
        )
        .parse(input)?;

    let moves = z.iter().flatten().copied().collect();
    Ok((input,moves))
}


fn read_input(input: &str) -> (Grid<Block>, Vec<Move>) {
    let (_, (grid, _, moves)) = (read_grid,line_ending,read_moves).parse(input).unwrap();
    (grid,moves)
}

fn find_robot(grid: &Grid<Block>) -> Point {
    grid.index_to_point(grid
            .iter()
            .enumerate()
            .find(|(_,b)| match b { Block::Robot => {true}, _ => {false}})
            .unwrap()
            .0)
}

fn embiggen(grid: &Grid<Block>) -> Grid<Block>
{
    Grid {
        cells:
            grid
                .cells
                .iter()
                .map(|block| match block {
                    Block::Box                          => { vec![Block::BoxLeft, Block::BoxRight]  },
                    Block::Empty | Block::Wall          => { vec![*block,*block]                    },
                    Block::Robot                        => { vec![Block::Robot, Block::Empty]       },
                    Block::BoxLeft | Block::BoxRight    => { panic!("No!")                          }
                })
                .collect::<Vec<_>>()
                .iter()
                .flatten()
                .map(|b| *b)
                .collect(),
        width: 2*grid.width,
        height: grid.height
    }
}

impl AOC for Day15 {
    fn part_one(&self, input: &str) -> String {
        
        let (mut x,moves) = read_input(input);
        //println!("{x}\n");

        for mv in moves {
            //print!("{:?}\n",mv);
            let robot = find_robot(&x);
            let can_move: bool;
            let mut current = robot.clone();
            let dir: Direction = mv.into();
            let mut counter: i64 = 0;

            current += dir;
            
            loop {
                let block = &x.get(current.x as usize,current.y as usize);
                match block {
                    Block::Box => { current += dir; counter += 1; },
                    Block::Empty => { can_move = true; break; },
                    Block::Wall => { can_move = false; break; },
                    Block::Robot | Block::BoxLeft | Block::BoxRight => { panic!("Never!"); }
                }
            }

            if can_move {
                let robot_index = x.point_to_index(&robot);
                x.cells[robot_index] = Block::Empty;
                let robot_index = x.point_to_index(&(robot + dir));
                x.cells[robot_index] = Block::Robot;

                if counter >= 1 {

                    let box_index = x.point_to_index(&(robot + dir * (counter + 1)));
                    x.cells[box_index] = Block::Box;

                }
            }
            //println!("{x}\n");
        }

        x
            .iter()
            .enumerate()
            .filter(|(_,b)| match b { Block::Box => { true }, _ => { false }})
            .map(|(i, _)| x.index_to_point(i))
            .map(|p| 100 * p.y + p.x)
            .sum::<i64>()
            .to_string()
    }

    fn part_two(&self, input: &str) -> String {
        
        let (x,moves) = read_input(input);

        let mut z = embiggen(&x);

        drop(x);

        for mv in moves {
            let robot = find_robot(&z);
            let mut current = robot.clone();

            //println!("{z}");

            // left and right moves are "easy" 

            if mv == Move::Left || mv == Move::Right {
                let dir: Direction = mv.into();
                let mut counter = 0;
                current += dir;
                let can_move: bool;
            
                loop {
                    let block = &z.get(current.x as usize,current.y as usize);
                    match block {
                        Block::BoxLeft | Block::BoxRight    => { current += dir; counter += 1;  },
                        Block::Empty                        => { can_move = true; break;        },
                        Block::Wall                         => { can_move = false; break;       },
                        Block::Robot | Block::Box           => { panic!("Never!");              }
                    }
                }

                if can_move {
                    counter += 1;
                    while counter > 0 {
                        let move_to = robot + dir * counter;
                        let move_from = move_to - dir;
                        *z.set(move_to.x as usize,move_to.y as usize) = *z.get(move_from.x as usize,move_from.y as usize);
                        counter -= 1;
                    }
                    *z.set(robot.x as usize, robot.y as usize) = Block::Empty;
                }
            } else {
                // up and down moves... heh 
                let dir: Direction = mv.into();
                let prev = current; // where robot is
                current += dir; 
                let next = *z.get(current.x as usize,current.y as usize);
                match next {
                    Block::Box | Block::Robot => { panic!("no") },
                    Block::Wall => { /* no op */ },
                    Block::Empty => { 
                        *z.set(current.x as usize, current.y as usize) = Block::Robot; 
                        *z.set(prev.x as usize,prev.y as usize) = Block::Empty; 
                    },
                    Block::BoxLeft | Block::BoxRight => {
                        // put the next box into the Move set, and then add its destination (s) to the Check set 
                        // always going up / down 1 level at a time 
                        let mut points_to_move = HashSet::<Point>::new();
                        let mut points_to_check = HashSet::<Point>::new(); 
                        let mut can_move = true;

                        points_to_move.insert(current);

                        if next == Block::BoxLeft {
                            points_to_move.insert(current + Move::Right.into());
                        } else {
                            points_to_move.insert(current + Move::Left.into());
                        }

                        for point in &points_to_move {
                            points_to_check.insert(*point + dir);
                        }
                        
                        points_to_move.insert(prev);

                        while can_move & !points_to_check.is_empty() {
                            let check_point = *points_to_check.iter().next().unwrap();
                            let check_block = *z.get(check_point.x as usize, check_point.y as usize);
                            match check_block {
                                Block::Wall => { can_move = false; break; }
                                Block::Robot | Block::Box => { panic!("no!!"); },
                                Block::Empty => { points_to_check.remove(&check_point); }
                                Block::BoxLeft => {
                                    points_to_move.insert(check_point);
                                    points_to_move.insert(check_point + Move::Right.into());
                                    points_to_check.insert(check_point + dir);
                                    points_to_check.insert(check_point + Move::Right.into() + dir);
                                    points_to_check.remove(&check_point);
                                }
                                Block::BoxRight => {
                                    points_to_move.insert(check_point);
                                    points_to_move.insert(check_point + Move::Left.into());
                                    points_to_check.insert(check_point + dir);
                                    points_to_check.insert(check_point + Move::Left.into() + dir);
                                    points_to_check.remove(&check_point);
                                }
                            }
                        }

                        if can_move {
                            let mut orig = HashMap::<Point,Block>::new();
                            for point in &points_to_move {
                                orig.entry(*point).or_insert(*z.get(point.x as usize, point.y as usize));
                            }
                            for point in &points_to_move {
                                *z.set(point.x as usize, point.y as usize) = Block::Empty;
                            }
                            for point in &points_to_move {
                                let new_point = *point + dir;
                                *z.set(new_point.x as usize, new_point.y as usize) = orig[point];
                            }
                        }
                    } // end big move for boxes
                }
            }
        }

        //println!("{z}");

        z
            .iter()
            .enumerate()
            .filter(|(_,b)| match b { Block::BoxLeft => { true }, _ => { false }})
            .map(|(i, _)| z.index_to_point(i))
            .map(|p| 100 * p.y + p.x)
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
        let test_file_path = "test-inputs/day15.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day15 {};
        assert_eq!(my_struct.part_one(&file),"10092");
    }

    #[test]
    fn part_1_2() {
        let test_file_path = "test-inputs/day15.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day15 {};
        assert_eq!(my_struct.part_one(&file),"2028");
    }
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day15.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day15 {};
        assert_eq!(my_struct.part_two(&file),"9021");
    }
    
    #[test]
    fn part_2_2() {
        let test_file_path = "test-inputs/day15.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day15 {};
        assert_eq!(my_struct.part_two(&file),"1751");
    }
    
    #[test]
    fn part_2_3() {
        let test_file_path = "test-inputs/day15.3.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day15 {};
        assert_eq!(my_struct.part_two(&file),"618");
    }
}