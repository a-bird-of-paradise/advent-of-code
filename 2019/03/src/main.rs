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

    let first_path = enpath(&first);
    let second_path = enpath(&second);

    let path_intersection : BTreeSet<(i64,i64)> = first_path.intersection(&second_path).cloned().collect();

    let mut answer_1 : i64 = std::i64::MAX;

    for it in path_intersection {
        let mut x = it.0; 
        let mut y = it.1;
        if x == 0 && y == 0 { continue; }
        if x < 0 { x = -x; }
        if y < 0 { y = -y; }
        if x + y < answer_1 { answer_1 = x + y; }
    }

    println!("{answer_1}");
}   

fn map_splitter(s : &str) -> (char,i64)
{
    let c : char = s.chars().nth(0).unwrap();
    let x : i64 = s[1..].parse::<i64>().expect("Could not parse integer");
    return (c,x);
}

fn enpath(def : &Vec<(char,i64)>) -> BTreeSet<(i64,i64)>
{
    let mut x : i64 = 0;
    let mut y : i64 = 0; 
    let mut answer : BTreeSet<(i64,i64)> = Default::default();

    answer.insert((x,y));

    for it in def {

        match it.0 {
            'L' =>  {   
                for _i in 0..it.1 {
                    x = x - 1;
                    answer.insert((x,y));
                }
            }
            'R' =>  {   
                for _i in 0..it.1 {
                    x = x + 1;
                    answer.insert((x,y));
                }
            }
            'U' =>  {   
                for _i in 0..it.1 {
                    y = y + 1;
                    answer.insert((x,y));
                }
            }
            'D' =>   {   
                for _i in 0..it.1 {
                    y = y - 1;
                    answer.insert((x,y));
                }
            }
            _ => panic!("Unknown move type")
        }
    }
    return answer;
}