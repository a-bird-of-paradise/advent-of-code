use std::cmp::Ordering;
use std::fs;
use std::env;
use std::collections::HashSet;
use std::collections::HashMap;
mod point;
mod ratio;

use crate::point::Point;
use crate::ratio::Ratio;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file: String = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let file : Vec<&str> = raw_file
        .trim()
        .split('\n')
        .collect();

    let points : HashSet<Point> = get_points(&file);
    let point_ratio_lookup: HashMap<Point, HashSet<Ratio>> = get_point_ratio_lookup(&points);
    let p1_point: (Point, usize) = part_1_answer(&point_ratio_lookup);
    println!("{:?}", &p1_point);

    let shifted_points: HashSet<Point> = get_shifted_points(&points, &p1_point.0);

    let mut ratio_distance_map: HashMap<Ratio, Vec<Point>> = get_ratio_distance_map(&shifted_points);

    let mut keys : Vec<Ratio> = ratio_distance_map.keys().cloned().collect();
    keys.sort();

    let mut current_key_index: usize = 0; 
    let mut current_point : Point = Point::new(0,0);
    let mut i : usize = 0; 

    while i < 200 { // can loop infinitely but live with it for now
        // does current key have any elements
        let current_key : Ratio = keys[current_key_index].clone();
        if ratio_distance_map[&current_key].len() > 0 {
            current_point = *ratio_distance_map[&current_key].first().unwrap();
            ratio_distance_map.entry(current_key).and_modify(|v| { v.remove(0); });
            i+=1; 
        }
        // tick current key
        current_key_index = (current_key_index + 1) % keys.len();
    }

    current_point = current_point+p1_point.0;
    let answer_2 : i32 = current_point.x * 100 + current_point.y;
    println!("{}",answer_2);

}

fn part_1_answer(point_ratio_lookup: &HashMap<Point, HashSet<Ratio>>) -> (Point,usize) {
    let mut answer : usize = 0; 
    let mut kanswer : Point = Point::new(0,0);

    for (key, value) in point_ratio_lookup {
        if value.len() > answer {
            answer = value.len();
            kanswer = *key;
        }
    }
    return (kanswer,answer - 1);
}

fn get_ratio_distance_map(points: &HashSet<Point>) -> HashMap<Ratio, Vec<Point>> {
    let mut ratio_distance_map : HashMap<Ratio, Vec<Point>> = HashMap::new();

    for point in points {
        let r : Ratio = point.to_ratio();
        ratio_distance_map.entry(r).or_insert_with(Vec::new).push(*point);
    }

    for (_key, value) in &mut ratio_distance_map {
        value.sort_by(|a,b| if a.sumsq() < b.sumsq() { Ordering::Less } else if a.sumsq() > b.sumsq() { Ordering::Greater } else { Ordering::Equal });
    }

    return ratio_distance_map;
}

fn get_point_ratio_lookup(points : &HashSet<Point>) -> HashMap<Point, HashSet<Ratio>>
{
    let mut point_ratio_lookup : HashMap<Point,HashSet<Ratio>> = HashMap::new();

    for key_point in points {
        for value_point in points {
            let dx = value_point.x - key_point.x;
            let dy = (value_point.y - key_point.y) * -1;
            point_ratio_lookup.entry(*key_point).or_insert_with(HashSet::new).insert(Ratio::new(dy, dx));
        }
    }
    return point_ratio_lookup;

}

fn get_points(file : &Vec<&str>) -> HashSet<Point>
{
    let mut points : HashSet<Point> = HashSet::new();

    for y in 0..file.len().try_into().unwrap() {
        for x in 0..file[y as usize].len().try_into().unwrap() {
            let xu: usize = x as usize;
            let yu: usize = y as usize;

            if file[yu].chars().nth(xu).unwrap() == '#' {
                points.insert(Point::new(x,y));
            }
        }
    }

    return points;
}

fn get_shifted_points(orig : &HashSet<Point>, base : &Point) -> HashSet<Point> {

    let mut points : HashSet<Point> = HashSet::new();

    for point in orig {
        if *point != *base {
            points.insert(*point - *base);
        }
    }

    return points;
}


#[test]
fn test_get_points() {
    let file : Vec<&str> = vec![
        ".#..#",
        ".....",
        "#####",
        "....#",
        "...##"
    ];
    let points: HashSet<Point> = get_points(&file);
    for point in points { println!("{:?}",point);}
}

#[test]
fn test_get_point_ratio_lookup() {
    let file : Vec<&str> = vec![
        ".#..#",
        ".....",
        "#####",
        "....#",
        "...##"
    ];
    let points: HashSet<Point> = get_points(&file);
    let point_ratio_lookup: HashMap<Point, HashSet<Ratio>> = get_point_ratio_lookup(&points);
    for point in points {
        println!("{:?} {}", point, point_ratio_lookup[&point].len());
    }
}