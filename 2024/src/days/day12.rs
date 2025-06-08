
use std::collections::{HashMap, BTreeSet};

use aoc2024::{Direction, Point, AOC};
pub struct Day12;

fn read_input(input: &str) -> HashMap<Point,char>
{
    let mut answer: HashMap<Point, char> = Default::default();

    let mut row = 0;
    let mut col = 0;

    for c in input.chars() {
        match c {
            'A'..='Z' => { answer.entry(Point{x:col, y:row}).or_insert(c); col += 1;   },
            '\r'      => {                                                                          },
            '\n'      => { row += 1; col = 0;                                                       },
            _         => { println!("Unrecognised character: {c}");                                 }
        }
    }

    answer
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
enum EdgeType {
    Left,
    Right,
    Up,
    Down
}

#[derive(Debug, Clone, PartialEq, Eq, Default)]
struct Region {
    content: char,
    points: BTreeSet<Point>
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct UnitEdgeSegment {
    // convention: direction is either (0,1) or (1,0) i.e. down or right
    origin: Point,
    dir: Direction,
    edgetype: EdgeType
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct EdgeSegment {
    // convention: direction is either (0,1) or (1,0) i.e. down or right
    origin: Point,
    dir: Direction,
    edgetype: EdgeType,
    length: usize
}

impl Region {
    pub fn area(&self) -> usize {
        self.points.len()
    }
    pub fn perimeter(&self) -> usize {
        let mut answer = 0usize;

        for p in &self.points {
            for n in p.cardinal_neighbours() {
                if !self.points.contains(&n) {
                    answer += 1;
                }
            }
        }
        answer
    }

    pub fn unit_edge_segments(&self) -> BTreeSet<UnitEdgeSegment> {
        let mut answer = BTreeSet::<UnitEdgeSegment>::new();
        for p in &self.points {

            // top edge
            if !self.points.contains(&(*p + Direction { x: 0, y: -1 })) {
                answer.insert(UnitEdgeSegment { origin: *p, dir: Direction { x: 1, y: 0 }, edgetype: EdgeType::Up });
            }

            // bottom edge
            if !self.points.contains(&(*p + Direction { x: 0, y: 1 })) {
                answer.insert(UnitEdgeSegment { origin: *p + Direction { x: 0, y: 1}, dir: Direction { x: 1, y: 0 }, edgetype: EdgeType::Down });
            }

            // left edge 
            if !self.points.contains(&(*p + Direction { x: -1, y: 0 })) {
                answer.insert(UnitEdgeSegment { origin: *p, dir: Direction { x: 0, y: 1 }, edgetype: EdgeType::Left });
            }

            // right edge
            if !self.points.contains(&(*p + Direction { x: 1, y: 0 })) {
                answer.insert(UnitEdgeSegment { origin: *p + Direction { x: 1, y: 01}, dir: Direction { x: 0, y: 1 }, edgetype: EdgeType::Right });
            }

        }
        answer
    }

    pub fn edge_segments (&self) -> BTreeSet<EdgeSegment> {
        let mut answer = BTreeSet::<EdgeSegment>::new();
        let mut units = self.unit_edge_segments();
        while !units.is_empty() {
            let mut current = units.iter().next().unwrap().clone();
            while units.contains(&UnitEdgeSegment { origin: current.origin - current.dir, dir: current.dir, edgetype: current.edgetype}) {
                current.origin -= current.dir;
            }
            // get length and consume
            let mut length = 0;
            let start = current.clone();
            while units.contains(&current) {
                length += 1;
                units.remove(&current);
                current.origin += current.dir;
            }
            answer.insert(EdgeSegment { origin: start.origin, dir: start.dir, length: length, edgetype: current.edgetype });
        }
        answer
    }
}

fn get_regions(input: &HashMap<Point,char>) -> Vec<Region> {
    let mut answer = Vec::<Region>::new();

    let mut unassigned = BTreeSet::<Point>::new();

    for (k, _) in input { unassigned.insert(*k);    }

    while !unassigned.is_empty() {
        let current = unassigned.iter().next().unwrap().clone();
        unassigned.remove(&current);
        let mut this_region = Region { content: input[&current], points: Default::default() };
        this_region.points.insert(current);
        let mut visited = BTreeSet::<Point>::new();
        let mut unvisited =  BTreeSet::<Point>::new();

        for neighbour in current.cardinal_neighbours() { unvisited.insert(neighbour); }

        while !unvisited.is_empty() {
            let candidate = unvisited.iter().next().unwrap().clone();
            unvisited.remove(&candidate);
            visited.insert(candidate);

            if input.contains_key(&candidate) {
                if input[&candidate] == this_region.content {
                    unassigned.remove(&candidate);
                    this_region.points.insert(candidate);
                    for neighbour in candidate.cardinal_neighbours() {
                        if !visited.contains(&neighbour) {
                            unvisited.insert(neighbour);
                        }
                    }
                }
            }
        }

        answer.push(this_region);
        visited.clear();
        unvisited.clear();

    }

    answer
}

impl AOC for Day12 {
    fn part_one(&self, input: &str) -> String {
        
        let regions = get_regions(&read_input(input));

        regions
            .iter()
            .map(|r| r.area() * r.perimeter())
            .sum::<usize>()
            .to_string()
    }
    
    fn part_two(&self, input: &str) -> String {

        let regions = get_regions(&read_input(input));

        regions
            .iter()
            .map(|r| r.area() * r.edge_segments().len())
            .sum::<usize>()
            .to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day12.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day12 {};
        assert_eq!(my_struct.part_one(&file),"1930")
    } 
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day12.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day12 {};
        assert_eq!(my_struct.part_two(&file),"1206");
    }
    
    #[test]
    fn part_2_2() {
        let test_file_path = "test-inputs/day12.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day12 {};
        assert_eq!(my_struct.part_two(&file),"236");
    }
    
    #[test]
    fn part_2_3() {
        let test_file_path = "test-inputs/day12.3.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day12 {};
        assert_eq!(my_struct.part_two(&file),"368");
    }
    
}