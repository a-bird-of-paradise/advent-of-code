use std::fs;
use std::env;
use intcode::Machine;
use intcode::MachineStatus;
use num_derive::FromPrimitive;
use num_traits::FromPrimitive;
use std::collections::BTreeMap;

mod intcode;
use intcode::do_intcode;

mod point;
use point::Point;

#[derive(FromPrimitive,Debug,PartialEq,Eq,Ord,PartialOrd)]
enum Tile {
    Empty = 0,
    Wall = 1,
    Block = 2,
    Paddle = 3,
    Ball = 4
}

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<i64> = raw_file
        .trim()
        .split(',')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    let mut numbers_2 = numbers.clone();

    let mut output : Vec<i64> = Vec::new();
    let input : Vec<i64> = Vec::new();

    let mut tiles : BTreeMap<Tile,Vec<Point>> = BTreeMap::new();
    let mut points : BTreeMap<Point,Tile> = BTreeMap::new();

    do_intcode(numbers, &input, &mut output);

    for chunk in output.chunks(3) {
        let key = Tile::from_i64(chunk[2]).unwrap();
        let key_2 = Tile::from_i64(chunk[2]).unwrap();
        let point = Point::new(chunk[0] as i32, chunk[1] as i32);

        tiles.entry(key).or_insert_with(Vec::new).push(point);
        points.entry(point).or_insert(key_2);
    }

    println!("{}", tiles[&Tile::Block].len());

    // part 2...

    numbers_2[0] = 2;

    println!("{}",part_2(&numbers_2,false)) // call with true for a neat animation

}

fn part_2(numbers: &Vec<i64>, noisy: bool) -> i64
{
    let mut machine : Machine = Machine::new(&numbers);
    let mut status : MachineStatus;

    let mut tiles : BTreeMap<Tile,Vec<Point>> = BTreeMap::new();
    let mut points : BTreeMap<Point,Tile> = BTreeMap::new();

    let mut score : i64 = 0;

    let sentinel : Point = Point::new(-1,0);

    let mut ball_prior_x = 0;

    loop {

        status = machine.run_to_block();

        match status {
            MachineStatus::Halted => { score = *machine.output.last().unwrap(); break },
            MachineStatus::WriteBlocked => continue,
            MachineStatus::ReadBlocked => {

                for chunk in machine.output.chunks(3) {
                    let point = Point::new(chunk[0] as i32, chunk[1] as i32);
                    if point != sentinel {
                        let key = Tile::from_i64(chunk[2]).unwrap();
                        let key_2 = Tile::from_i64(chunk[2]).unwrap();

                        if key == Tile::Empty {
                            points.retain(|k, _v| *k != point);
                            
                            for (_k, v) in &mut tiles {
                                v.retain(|p| *p != point);
                            }

                        } else { 
                            tiles.entry(key).or_insert_with(Vec::new).push(point);
                            points.entry(point).or_insert(key_2);
                        }
                    } else {
                        score = chunk[2];
                    }
                }
                if noisy {
                    print(&points);
                    println!("Score: {score}\n");
                }

                let ball_x = tiles.get(&Tile::Ball).unwrap().get(0).unwrap().x;
                let padd_x = tiles.get(&Tile::Paddle).unwrap().get(0).unwrap().x;

                let input : i64 ;

                if ball_x > ball_prior_x {
                    if padd_x >= ball_x { input = 0 } else { input = 1 }
                } else {
                    if padd_x >= ball_x { input = -1 } else { input = 0 }
                }

                ball_prior_x = ball_x;

                machine.input.push(input);
                machine.output.clear();
            }
        }
    }
    return score;
}

fn print(points: &BTreeMap<Point,Tile>) {
    std::process::Command::new("clear").status().unwrap();
    let max_x = points.keys().max_by(|a,b| a.x.cmp(&b.x)).unwrap().x;
    let max_y = points.keys().max_by(|a,b| a.y.cmp(&b.y)).unwrap().y;

    for y in 0..=max_y {
        for x in 0..=max_x {
            let mut c : char = ' ';
            let p = Point::new(x,y);
            if points.contains_key(&p) {
                let t = points.get(&p).unwrap();

                c = match t {
                    Tile::Ball => 'O',
                    Tile::Block => '#',
                    Tile::Empty => ' ',
                    Tile::Paddle => '-',
                    Tile::Wall => '*'
                }
            }
            print!("{c}");
        }
        print!("\n");
    }
}