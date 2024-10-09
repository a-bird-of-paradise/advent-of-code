use std::collections::HashMap;
use std::fs;
use std::env;
use std::collections::BTreeSet;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");

    let lines : Vec<&str> = raw_file.trim().split('\n').collect();

    let first : Vec<(char,i64)> = lines[0]
        .trim()
        .split(',')
        .map(|s| map_splitter(s))
        .collect();

    let second : Vec<(char,i64)> = lines[1]
        .trim()
        .split(',')
        .map(|s| map_splitter(s))
        .collect();

    let first_path: HashMap<(i64,i64), i64> = enpath(&first);
    let second_path: HashMap<(i64,i64), i64> = enpath(&second);

    let mut first_path_points : BTreeSet<(i64,i64)> = Default::default();
    let mut second_path_points : BTreeSet<(i64,i64)> = Default::default();

    for pt in &first_path { first_path_points.insert((pt.0.0,pt.0.1)); }
    for pt in &second_path { second_path_points.insert((pt.0.0,pt.0.1)); }

    let path_intersection : BTreeSet<(i64,i64)> = first_path_points.intersection(&second_path_points).cloned().collect();

    let mut answer_1 : i64 = std::i64::MAX;
    let mut answer_2 : i64 = std::i64::MAX;

    for it in path_intersection {
        let mut x = it.0; 
        let mut y = it.1;
        if x == 0 && y == 0 { continue; }

        if first_path[&(x,y)] + second_path[&(x,y)] < answer_2 { answer_2 = first_path[&(x,y)] + second_path[&(x,y)]; }

        if x < 0 { x = -x; }
        if y < 0 { y = -y; }
        if x + y < answer_1 { answer_1 = x + y; }
    }

    println!("{answer_1}");
    println!("{answer_2}");
}   

fn map_splitter(s : &str) -> (char,i64)
{
    let c : char = s.chars().nth(0).unwrap();
    let x : i64 = s[1..].parse::<i64>().expect("Could not parse integer");
    return (c,x);
}

fn enpath(def : &Vec<(char,i64)>) -> HashMap<(i64,i64), i64>
{
    let mut x : i64 = 0;
    let mut y : i64 = 0; 
    let mut dist : i64 = 0;
    let mut answer : HashMap<(i64,i64), i64> = HashMap::new();

    for it in def {

        match it.0 {
            'L' =>  {   
                for _i in 0..it.1 {
                    x = x - 1;
                    dist = dist + 1;
                    answer.insert((x,y),dist);
                }
            }
            'R' =>  {   
                for _i in 0..it.1 {
                    x = x + 1;
                    dist = dist + 1;
                    answer.insert((x,y),dist);
                }
            }
            'U' =>  {   
                for _i in 0..it.1 {
                    y = y + 1;
                    dist = dist + 1;
                    answer.insert((x,y),dist);
                }
            }
            'D' =>   {   
                for _i in 0..it.1 {
                    y = y - 1;
                    dist = dist + 1;
                    answer.insert((x,y),dist);
                }
            }
            _ => panic!("Unknown move type")
        }
    }
    return answer;
}