use std::{cmp::Ordering, collections::{BinaryHeap, HashMap, HashSet}};

use aoc2024::{AOC,Point,Direction};
use cached::proc_macro::cached;

pub struct Day16;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct Node {
    pub location: Point,
    pub direction: Direction
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
struct State {
    pub node: Node,
    pub cost: usize
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct Edge {
    pub start_node: Node,
    pub end_node: Node,
    pub cost: usize
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct Grid {
    pub reachable_points: HashSet<Point>,
    pub start_point: Point,
    pub end_point: Point
}

fn connectivity_graph(nodes: &HashSet<Node>) -> HashMap<Node, Vec<Edge>> {
    let mut graph = HashMap::<Node,Vec<Edge>>::new();

    for node in nodes {
        let dir = node.direction;
        let pt = node.location;
        let neighbours: Vec<Node> = nodes
            .iter()
            .filter(|s| (s.location == pt && s.direction != dir ) || (s.location == dir + pt && s.direction == dir))
            .map(|s| *s)
            .collect();
        let edges: Vec<Edge> = neighbours
            .iter()
            .map(|p| Edge { 
                start_node: *node, 
                end_node: *p, 
                cost: if p.location == pt { if p.direction == -dir { 2000 } else  {1000} } else { 1 } })
            .collect();

        graph.entry(*node).or_insert(edges);
    }
    graph
}

fn reachable_nodes(grid: &Grid) -> HashSet<Node> {
    let mut answer: HashSet<Node> = Default::default();

    let origin = Point { x: 0, y: 0 };
    let cardinal_points = origin.cardinal_neighbours();
    let cardinal_directions: Vec<Direction> = cardinal_points.iter().map(|p| *p - origin).collect();

    for point in &grid.reachable_points {
        for dir in &cardinal_directions {
            if grid.reachable_points.contains(&(*point + *dir)) {
                answer.insert(Node { location: *point, direction: *dir }); // leave that way
                answer.insert(Node { location: *point, direction: -*dir}); // enter that way
            }
        }
    }

    answer.insert(Node { location: grid.start_point, direction: Direction { x: 1, y: 0 } } );

    answer
}

fn read_input(input: &str) -> Grid {
    let mut row = 0i64; 
    let mut col = 0i64;
    let mut s_loc: Point = Default::default();
    let mut e_loc: Point = Default::default();
    let mut points = HashSet::<Point>::new();

    for c in input.chars() {
        match c {
            '#'     => { col += 1;                                          },
            '\r'    => {                                                    },
            '\n'    => { col = 0; row += 1;                                 },
            'S'     => { s_loc = Point { x: col, y: row }; col += 1;        },
            'E'     => { e_loc = Point { x: col, y: row }; col += 1;        },
            '.'     => { points.insert(Point { x: col, y: row}); col += 1;  },
            _       => { panic!("Oh No!");                                  }
        }
    }

    points.insert(s_loc);
    points.insert(e_loc);

    Grid { reachable_points: points, start_point: s_loc, end_point: e_loc }

}

fn distances(grid: &Grid) -> HashMap<Node,usize> {
    let rs = reachable_nodes(&grid);
    let cg = connectivity_graph(&rs);
    let start_node = Node { location: grid.start_point, direction: Direction { x: 1, y: 0 }};

    let mut distance : HashMap<Node,usize> = Default::default();
    let mut unvisited : BinaryHeap<State> = Default::default();
    let mut least_distance_to_end = usize::MAX;

    distance.entry(start_node).or_insert(0);

    for node in &rs { 
        distance.entry(*node).or_insert(usize::MAX);
    }

    unvisited.push(State { node: start_node, cost: 0 });

    while !unvisited.is_empty() {
        let smallest_unvisited_state = unvisited.pop().unwrap();

        if smallest_unvisited_state.node.location == grid.end_point && smallest_unvisited_state.cost < least_distance_to_end {
            least_distance_to_end = smallest_unvisited_state.cost;
        }

        for edge in &cg[&smallest_unvisited_state.node] {
            let candidate_cost = *distance.get(&smallest_unvisited_state.node).unwrap() + edge.cost;
            if candidate_cost < *distance.get(&edge.end_node).unwrap() && candidate_cost < least_distance_to_end {
                distance.entry(edge.end_node).and_modify(|v| *v = candidate_cost);
                unvisited.push(State{ node: edge.end_node, cost: candidate_cost});
            }
        }
    }
    distance
}

#[cached] // can't just use static std::map<key,value> to cache results in rust
fn distances_driver(input: String) -> (Grid,HashMap<Node,usize>) {
    let g = read_input(&input);
    (g.clone(),distances(&g))
}

impl AOC for Day16 {

    fn part_one(&self, input: &str) -> String {
        let (g,distance) = distances_driver(input.to_string());

        distance
            .iter()
            .filter(|(s,_)| s.location == g.end_point)
            .map(|(_s,u)| *u)
            .min()
            .unwrap()
            .to_string()
    }
    
    fn part_two(&self, input: &str) -> String {

        let (g,distance) = distances_driver(input.to_string());

        let target_distance = 
            distance
            .iter()
            .filter(|(s,_)| s.location == g.end_point)
            .map(|(_s,u)| *u)
            .min()
            .unwrap();

        let terminal_nodes: Vec<_> = 
            distance
            .iter()
            .filter(|(n,d)| n.location == g.end_point && **d == target_distance)
            .collect();

        let first_state = State { node: *terminal_nodes.first().unwrap().0, cost: *terminal_nodes.first().unwrap().1 };

        let mut state_stack = Vec::<State>::new();
        state_stack.push(first_state);

        let mut state_seen: HashSet<State> = Default::default();

        while !state_stack.is_empty() {
            let current_state = state_stack.pop().unwrap();
            if !state_seen.contains(&current_state) {
                state_seen.insert(current_state);
                
                let current_cost = current_state.cost; 

                if current_cost > 1000 {
                    // rot right
                    
                    let candidate_state = State {
                        node: Node {
                            location: current_state.node.location,
                            direction: current_state.node.direction.rot_right()
                        },
                        cost: current_cost - 1000
                    };
                    if distance.contains_key(&candidate_state.node) && distance[&candidate_state.node] == candidate_state.cost {
                        state_stack.push(candidate_state);
                    }

                    // rot left 
                    
                    let candidate_state = State {
                        node: Node {
                            location: current_state.node.location,
                            direction: current_state.node.direction.rot_right().rot_right().rot_right()
                        },
                        cost: current_cost - 1000
                    };
                    if distance.contains_key(&candidate_state.node) && distance[&candidate_state.node] == candidate_state.cost {
                        state_stack.push(candidate_state);
                    }
                }

                if current_cost > 0 {
                    let candidate_state = State {
                        node: Node {
                            location: current_state.node.location - current_state.node.direction,
                            direction: current_state.node.direction
                        },
                        cost: current_cost - 1
                    };
                    if distance.contains_key(&candidate_state.node) && distance[&candidate_state.node] == candidate_state.cost {
                        state_stack.push(candidate_state);
                    }
                }
            }
        }

        let path_points: HashSet<Point> = state_seen.iter().map(|s| s.node.location).collect();

        path_points.len().to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day16.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day16 {};
        assert_eq!(my_struct.part_one(&file),"7036");
    }

    #[test]
    fn part_1_2() {
        let test_file_path = "test-inputs/day16.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day16 {};
        assert_eq!(my_struct.part_one(&file),"11048");
    }
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day16.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day16 {};
        assert_eq!(my_struct.part_two(&file),"45");
    }
    
    #[test]
    fn part_2_2() {
        let test_file_path = "test-inputs/day16.2.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day16 {};
        assert_eq!(my_struct.part_two(&file),"64");
    }
    
}