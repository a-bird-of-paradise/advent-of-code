use std::cmp::Ordering;
use std::fs;
use std::env;
use std::i32;
use intcode::{Machine,MachineStatus};
use std::collections::{BTreeMap,BTreeSet,VecDeque,BinaryHeap};

mod intcode;

mod point;
use point::Point;

fn main () {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let mut path_graph : BTreeMap<Point,BTreeSet<Point>> = BTreeMap::new();
    let mut wall_points : BTreeSet<Point> = BTreeSet::new();
    let mut path_points : BTreeSet<Point> = BTreeSet::new();
    let mut oxygen_point : Point = Point::new(0,0);
    let mut to_visit : VecDeque<Point> = VecDeque::new();

    let mut machine : Machine = Machine::new(&numbers);
    let mut status : MachineStatus;

    let mut current_point : Point = Point::new(0, 0);

    let mut oxygen_point_found: bool = false;

    to_visit.push_back(current_point + Point { x : 0 , y : 1 } );
    to_visit.push_back(current_point + Point { x : 0 , y : -1 } );
    to_visit.push_back(current_point + Point { x : 1 , y : 0 } );
    to_visit.push_back(current_point + Point { x : -1 , y : 0 } );

    let p_up = Point::new(0,1);
    let p_down = Point::new(0,-1);
    let p_left = Point::new(-1,0);
    let p_right = Point::new(1,0);

    path_points.insert(current_point);

    while !to_visit.is_empty() {

        if !current_point.is_neighbour(to_visit.front().unwrap()) {
            let mut destination : Point = Point::new(i32::MAX,i32::MAX);
            let mut found = false;
            for point in &path_points {
                if point.is_neighbour(to_visit.front().unwrap()) {
                    destination = *point;
                    found = true;
                    break;
                }
            } 
            if !found { panic!("Oh No!") }
            
            let mut path = find_path(&path_graph, current_point, destination);

            path.pop();

            while !path.is_empty() {
                let diff = *path.last().unwrap() - current_point;
                path.pop();

                let command : i64;
                command = if diff == p_up { 1 } else if diff == p_down { 2 } else if diff == p_left { 3 } else if diff == p_right { 4 } else { panic!("Oh no!!")};
                machine.input.push(command);
                status = machine.run_to_block();
                assert!(status == MachineStatus::WriteBlocked);
                assert!(*machine.output.last().unwrap() != 0);
                current_point = current_point + diff;
            }
        }

        let diff = *to_visit.front().unwrap() - current_point;

        let command : i64;
        command = if diff == p_up { 1 } else if diff == p_down { 2 } else if diff == p_left { 3 } else if diff == p_right { 4 } else { panic!("Oh no!!")};

        machine.input.push(command);

        status = machine.run_to_block();

        assert!(status == MachineStatus::WriteBlocked);

        let output = *machine.output.last().unwrap();

        if output == 0 {
            wall_points.insert(*to_visit.front().unwrap());
            to_visit.pop_front();
        } else {

            if output == 2 {
                
                if oxygen_point_found { panic!("Shouldn't happen!") }

                oxygen_point = current_point;
                oxygen_point_found = true;
            }

            path_points.insert(*to_visit.front().unwrap());
            path_graph.entry(current_point).or_insert_with(|| BTreeSet::new()).insert(*to_visit.front().unwrap());
            path_graph.entry(*to_visit.front().unwrap()).or_insert_with(|| BTreeSet::new()).insert(current_point);
            to_visit.pop_front();

            let new_point = current_point + diff;
            let up_point = new_point + p_up;
            let down_point = new_point + p_down;
            let left_point = new_point + p_left;
            let right_point = new_point + p_right;

            if !path_points.contains(&up_point) && !wall_points.contains(&up_point) { to_visit.push_front(up_point);}
            if !path_points.contains(&down_point) && !wall_points.contains(&down_point) { to_visit.push_front(down_point);}
            if !path_points.contains(&left_point) && !wall_points.contains(&left_point) { to_visit.push_front(left_point);}
            if !path_points.contains(&right_point) && !wall_points.contains(&right_point) { to_visit.push_front(right_point);}

            current_point = new_point;
        } 
    }

    let to_oxy = find_path(&path_graph, Point { x: 0, y: 0 }, oxygen_point);
/*
    for y in -30..30 {
        for x in -30..30 {
            let point = Point::new(x, -y);
            let is_wall = wall_points.contains(&point);
            let is_path = path_points.contains(&point); 
            let is_origin = x == 0 && y == 0;
            let is_oxy = point == oxygen_point;
            let is_on_path = to_oxy.contains(&point);
            let char = if is_origin { 'O' }
                else if is_oxy { '0' }
                else if is_on_path { '.' }
                else if !is_wall & !is_path { ' ' } 
                else if is_wall & !is_path { '#' } 
                else if !is_wall & is_path { ' ' }
                else { 'X' };

            print!("{char}");
        }
        print!("\n");
    }
*/

    println!("{:?}",to_oxy.len());

    let mut distance : BTreeMap<Point,i32> = BTreeMap::new();

    flood(&mut path_points, oxygen_point, &mut distance,0);

    println!("{:?}",distance.values().max().unwrap()+1);
/*
    for y in -30..30 {
        for x in -30..30 {
            let point = Point::new(x, -y);
            let is_wall = wall_points.contains(&point);
            let is_path = distance.contains_key(&point); 
            let is_origin = x == 0 && y == 0;
            let is_oxy = point == oxygen_point;
            if is_origin { print!(" O   ") }
                else if is_oxy { print!(" 0   ") }
                else if is_path { print!(" {:<4}", distance.get(&point).unwrap()) }
                else if !is_wall & !is_path { print!("     ") } 
                else if is_wall & !is_path { print!(" #   ") } 
                else if !is_wall & is_path { print!("    ") }
                else { print!(" X  ") };

        }
        print!("\n");
    }
*/

}

fn flood (points: &mut BTreeSet<Point>, origin: Point, answer: &mut BTreeMap<Point,i32>, depth: i32)
{
    points.remove(&origin);
    answer.entry(origin).or_insert(depth);

    let p_up = Point::new(0,1);
    let p_down = Point::new(0,-1);
    let p_left = Point::new(-1,0);
    let p_right = Point::new(1,0);

    let up_point = origin + p_up;
    let down_point = origin + p_down;
    let left_point = origin + p_left;
    let right_point = origin + p_right;

    if points.contains(&up_point) { flood(points,up_point,answer,depth+1); }
    if points.contains(&down_point) { flood(points,down_point,answer,depth+1); }
    if points.contains(&left_point) { flood(points,left_point,answer,depth+1); }
    if points.contains(&right_point) { flood(points,right_point,answer,depth+1); }

}

#[derive(Copy, Clone, Eq, PartialEq, Debug)]
struct State {
    cost: i32,
    point: Point
}

impl Ord for State { // for min heap 
    fn cmp(&self, other: &Self) -> Ordering {
        other.cost.cmp(&self.cost)
            .then_with(|| self.point.cmp(&other.point))
    }
}

impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

fn find_path(graph: &BTreeMap<Point,BTreeSet<Point>>, source: Point, destination: Point) -> Vec<Point> {
    let mut path : Vec<Point> = Vec::new();

    let mut open_set : BinaryHeap<State> = BinaryHeap::new();
    let mut came_from : BTreeMap<Point,Point> = BTreeMap::new();
    let mut gscore : BTreeMap<Point,i32> = BTreeMap::new();
    let mut fscore : BTreeMap<Point,i32> = BTreeMap::new();

    open_set.push(State{ cost: 0, point: source });

    for key in graph.keys() { gscore.insert(*key, i32::MAX); }
    gscore.entry(source).and_modify(|e| *e = 0 );

    for key in graph.keys() { fscore.insert(*key, i32::MAX); }
    fscore.entry(source).and_modify(|e| *e = source.manhattan(&destination));

    while !open_set.is_empty() {

        let mut current = *open_set.peek().unwrap();

        if current.point == destination { 
            path.push(current.point);
            while came_from.contains_key(&current.point) {
                current.point = *came_from.get(&current.point).unwrap();
                path.push(current.point);
            }
            break;
        }

        open_set.pop();

        for neighbour in graph.get(&current.point).unwrap() {
            let tentative_gscore = gscore.get(&current.point).unwrap() + 1;
            if tentative_gscore < *gscore.get(neighbour).unwrap() {
                *came_from.entry(*neighbour).or_insert(Point{x:0,y:0}) = current.point;
                gscore.entry(*neighbour).and_modify(|v| *v = tentative_gscore );
                fscore.entry(*neighbour).and_modify(|v| *v = tentative_gscore + neighbour.manhattan(&current.point)) ;

                let mut seen: bool = false;

                for item in &open_set {
                    if item.point == *neighbour {
                        seen = true;
                        break;
                    }
                }

                if !seen {
                    open_set.push(State { cost : *fscore.get(neighbour).unwrap(), point: *neighbour } );
                }
            }
        }
    }
    path
}

#[test]
fn test_find_path() {

    let source: Point = Point::new(3, 4);
    let destination: Point = Point::new(-3, -4);
    let mut graph: BTreeMap<Point,BTreeSet<Point>> = BTreeMap::new();

    let p_up = Point::new(0,1);
    let p_down = Point::new(0,-1);
    let p_left = Point::new(-1,0);
    let p_right = Point::new(1,0);

    for x in -5..5 {
        for y in -5..5 {
            let current = Point::new(x,y);
            let up_point = current + p_up;
            let down_point = current + p_down;
            let left_point = current + p_left;
            let right_point = current + p_right;

            graph.entry(current).or_insert_with(BTreeSet::new).insert(up_point);
            graph.entry(current).or_insert_with(BTreeSet::new).insert(down_point);
            graph.entry(current).or_insert_with(BTreeSet::new).insert(left_point);
            graph.entry(current).or_insert_with(BTreeSet::new).insert(right_point);

            graph.entry(up_point).or_insert_with(BTreeSet::new).insert(current);
            graph.entry(down_point).or_insert_with(BTreeSet::new).insert(current);
            graph.entry(left_point).or_insert_with(BTreeSet::new).insert(current);
            graph.entry(right_point).or_insert_with(BTreeSet::new).insert(current);

        }
    }

    let path = find_path(&graph, source, destination);

    println!("{:?}",path);
    
    assert_eq!(path.len(), (source.manhattan(&destination)+1).try_into().unwrap());


}