use aoc2024::{Direction, Grid, Point, AOC};
use std::{cmp::Ordering,collections::{BinaryHeap,HashMap}, hash::Hash, i64};
use nom::{bytes::complete::tag, character::complete::{usize, line_ending}, combinator::opt, multi::many1, sequence::{separated_pair, terminated}, IResult, Parser};
pub struct Day18;

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Copy)]
enum SpaceStatus 
{
    Safe,
    Corrupted
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
struct State {
    pub node: Point,
    pub cost: i64
}

impl Ord for State {
    fn cmp(&self, other: &Self) -> Ordering {
        other.cost.cmp(&self.cost).then_with(|| self.node.cmp(&other.node))
    }
}

impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl std::fmt::Display for SpaceStatus {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            SpaceStatus::Corrupted => { write!(f, "#") },
            SpaceStatus::Safe => { write!(f, ".") }
        }
    }
}

fn read_line(input: &str) -> IResult<&str,(usize,usize)> {
    let (input, p) = terminated(separated_pair(usize, tag(","), usize),opt(line_ending)).parse(input)?;
    Ok((input,p))
}

fn read_input(input: &str) -> (Grid<SpaceStatus>, Vec<Point>) {
    let (_, v) = many1(read_line).parse(input).unwrap();
    let max_x = v
        .iter()
        .map(|e| e.0)
        .max()
        .unwrap();
    let max_y = v
        .iter()
        .map(|e| e.1)
        .max()
        .unwrap();

    let grid = Grid::<SpaceStatus> {cells: vec![SpaceStatus::Safe; (max_x+1) * (max_y+1)], width: max_x+1, height: max_y+1};

    (grid, v
        .iter()
        .map(|e| Point { x: e.0 as i64, y: e.1 as i64})
        .collect())
}

fn manhattan(a: Point, b: Point) -> i64 {
    (a.x - b.x).abs() + (a.y - b.y).abs()
}

fn make_path(came_from: &HashMap<Point,Point>, current: Point) -> Vec<Point> {
    let mut path = Vec::<Point>::new();
    let mut current = current;
    path.push(current);
    while came_from.contains_key(&current) {
        current = came_from[&current];
        path.push(current);
    }
    path
}

fn a_star(start: Point, end: Point, grid: &Grid<SpaceStatus>) -> Option<Vec<Point>> {

    let mut open_set : BinaryHeap<State> = Default::default();
    let mut came_from : HashMap<Point,Point> = Default::default();
    let mut g_score : HashMap<Point,i64> = Default::default();
    let mut f_score : HashMap<Point,i64> = Default::default();

    g_score.entry(start).or_insert(0);
    f_score.entry(start).or_insert(manhattan(start, end));
    open_set.push(State { node: start, cost: 0});

    let directions = vec![
        Direction { x: 1, y: 0 },
        Direction { x:-1, y: 0 },
        Direction { x: 0, y: 1 },
        Direction { x: 0, y:-1 }
    ];

    while !open_set.is_empty() {
        let current = open_set.pop().unwrap();
        if current.node == end {
            return Some(make_path(&came_from, current.node));
        }

        for dir in &directions {
            let neighbour = current.node + *dir; 
            if grid.contains(&neighbour) {
                if *grid.get(neighbour.x as usize, neighbour.y as usize) == SpaceStatus::Safe {
                    let tentative_g_score = g_score[&current.node] + manhattan(current.node, neighbour);
                    g_score.entry(neighbour).or_insert(i64::MAX);
                    f_score.entry(neighbour).or_insert(i64::MAX);
                    if tentative_g_score < g_score[&neighbour] {
                        *came_from.entry(neighbour).or_default() = current.node;
                        g_score.entry(neighbour).and_modify(|e| *e = tentative_g_score);
                        f_score.entry(neighbour).and_modify(|e| *e = tentative_g_score + manhattan(neighbour, end));
                        if !open_set.iter().any(|e| e.node == neighbour) {
                            open_set.push(State { node: neighbour, cost: f_score[&neighbour] });
                        }
                    }
                }
            }
        }
    }
    None
}


impl AOC for Day18{
    fn part_one(&self, input: &str) -> String {
        let (mut g,v) = read_input(input);

        let limit: usize = if g.width < 10 { 12 } else { 1024 };

        for i in 0..limit {
            *g.set(v[i].x as usize, v[i].y as usize) = SpaceStatus::Corrupted;
        }

        let start = Point { x: 0, y: 0 };
        let end = Point { x: g.width as i64 - 1, y: g.height as i64 - 1};
        let path = a_star(start, end, &g);

        if let Some(x) = &path {
            (x.iter().len()-1).to_string()
        } else {
            "failed".to_string()
        }
    }

    fn part_two(&self, input: &str) -> String {
        
        let (mut g,v) = read_input(input);

        let start = Point { x: 0, y: 0 };
        let end = Point { x: g.width as i64 - 1, y: g.height as i64 - 1};
        let mut path = a_star(start, end, &g);
        let mut v: Vec<Point> = v.iter().copied().rev().collect();
        let mut corrupted_point = v.pop().unwrap();

        while let Some(x) = &path {
            while !x.contains(&corrupted_point) {
                *g.set(corrupted_point.x as usize, corrupted_point.y as usize) = SpaceStatus::Corrupted;
                corrupted_point = v.pop().unwrap();
            }
            *g.set(corrupted_point.x as usize, corrupted_point.y as usize) = SpaceStatus::Corrupted;
            path = a_star(start, end, &g);
        }
        format!("{},{}",corrupted_point.x,corrupted_point.y)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day18.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day18 {};
        assert_eq!(my_struct.part_one(&file),"22");
    }
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day18.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day18 {};
        assert_eq!(my_struct.part_two(&file),"6,1");
    }
}