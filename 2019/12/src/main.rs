use std::cmp::Ordering;
use std::env;
use std::fs;
use nom::character::complete::newline;
use nom::combinator::opt;
use nom::IResult;
use nom::bytes::complete::tag;
use nom::character::complete::i32;
use nom::sequence::tuple;
use nom::multi::count;
use itertools::Itertools;
use std::collections::HashSet;
use std::iter::Iterator;
use num::integer::lcm;

#[derive(Debug,PartialEq,Eq,Clone,Copy,Ord,PartialOrd)]
pub struct Position {
  pub x:    i32,
  pub y:    i32,
  pub z:    i32
}
#[derive(Debug,PartialEq,Eq,Clone,Ord,PartialOrd,Copy)]
pub struct Velocity {
  pub vx:    i32,
  pub vy:    i32,
  pub vz:    i32
}
#[derive(Debug,PartialEq,Eq,Clone,Ord,PartialOrd,Copy)]
pub struct State {
    pub pos:    Position,
    pub vel:    Velocity,
}

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let positions = parse_four_position(&raw_file).unwrap().1;

    let states_orig = get_states(&positions);
    let mut states_1 = states_orig.clone();

    for _i in 0..1000 { tick(&mut states_1); }
    println!("{}",answer_1(&states_1));

    let mut states_2 = states_orig.clone();

    let mut state_cache: Vec<HashSet<Vec<i32>>> = Vec::new();
    for _i in 0..3 { state_cache.push(HashSet::new()); }
    let mut cycled: Vec<bool> = Vec::new();
    for _i in 0..3 { cycled.push(false); }
    let mut cycled_index: Vec<usize> = Vec::new();
    for _i in 0..3 { cycled_index.push(0); }
    let mut index : usize = 0;

    while cycled.iter().any(|x| !x) {
        let mut state_x : Vec<i32> = Vec::new();
        let mut state_y : Vec<i32> = Vec::new();
        let mut state_z : Vec<i32> = Vec::new();
        for moon in 0usize..4usize {
            state_x.push(states_2[moon].pos.x);
            state_x.push(states_2[moon].vel.vx);
            state_y.push(states_2[moon].pos.y);
            state_y.push(states_2[moon].vel.vy);
            state_z.push(states_2[moon].pos.z);
            state_z.push(states_2[moon].vel.vz);
        }
        
        for dim in 0usize..3usize {
            let key = if dim == 0 { &state_x } else if dim == 1 {&state_y } else {&state_z };
            
            if !cycled[dim] {
                if state_cache[dim].contains(key) {
                    cycled[dim] = true;
                    cycled_index[dim] = index;
                } else {
                    state_cache[dim].insert(key.clone());
                }
            }
        }
        index += 1;
        tick(&mut states_2);
    }

    println!("{}",lcm(lcm(cycled_index[0],cycled_index[1]),cycled_index[2])); 

}

fn answer_1(states: &Vec<State>) -> i32 { states.iter().map(|s| total_energy(s)).sum() }

fn total_energy(state: &State) -> i32 { kinetic_energy(state) * potential_energy(state) }
fn kinetic_energy(state: &State) -> i32 { state.vel.vx.abs() + state.vel.vy.abs() + state.vel.vz.abs() }
fn potential_energy(state: &State) -> i32 { state.pos.x.abs() + state.pos.y.abs() + state.pos.z.abs() }

fn get_states(positions: &Vec<Position>) -> Vec<State> {
    let mut states : Vec<State> = Vec::new();
    for i in 0..4 { states.push(State { pos: positions[i] , vel: Velocity { vx:0, vy:0, vz:0 } }); }
    return states;
}

fn tick(states : &mut Vec<State>) {

    for (i,j) in (0usize..4usize).tuple_combinations() {
        match states[i].pos.x.cmp(&states[j].pos.x) {
            Ordering::Equal   => {},
            Ordering::Less    => { states[i].vel.vx += 1; states[j].vel.vx -= 1; },
            Ordering::Greater => { states[i].vel.vx -= 1; states[j].vel.vx += 1; }
        }
        match states[i].pos.y.cmp(&states[j].pos.y) {
            Ordering::Equal   => {},
            Ordering::Less    => { states[i].vel.vy += 1; states[j].vel.vy -= 1; },
            Ordering::Greater => { states[i].vel.vy -= 1; states[j].vel.vy += 1; }
        }
        match states[i].pos.z.cmp(&states[j].pos.z) {
            Ordering::Equal   => {},
            Ordering::Less    => { states[i].vel.vz += 1; states[j].vel.vz -= 1; },
            Ordering::Greater => { states[i].vel.vz -= 1; states[j].vel.vz += 1; }
        }
    }

    for i in 0usize..4usize {
        states[i].pos.x += states[i].vel.vx;
        states[i].pos.y += states[i].vel.vy;
        states[i].pos.z += states[i].vel.vz;
    }

}

fn parse_four_position(input:&str) -> IResult<&str,Vec<Position>> { count(parse_position,4)(input) }

fn parse_position(input: &str) -> IResult<&str, Position> {
    let (remaining, parse) = tuple((
        tag("<x="),
        i32,
        tag(", y="),
        i32,
        tag(", z="),
        i32,
        tag(">"),
        opt(tag("\r")),
        newline
    ))(input)?;

    Ok((remaining, Position {x: parse.1, y: parse.3, z: parse.5}))
}

#[test]
fn test_parse_position() {
    let to_read : &str = 
"<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>
";

let post_1 = parse_position(to_read);
let post_2 = parse_position(post_1.as_ref().unwrap().0);
let post_3 = parse_position(post_2.as_ref().unwrap().0);
let post_4 = parse_position(post_3.as_ref().unwrap().0);

assert_eq!(post_1.unwrap().1, Position { x:-1, y: 0, z:2});
assert_eq!(post_2.unwrap().1, Position { x:2, y: -10, z:-7});
assert_eq!(post_3.unwrap().1, Position { x:4, y: -8, z:8});
assert_eq!(post_4.unwrap().1, Position { x:3, y: 5, z:-1});
}

#[test]
fn test_parse_four_position() {
    let to_read : &str = 
"<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>
";

let answer = parse_four_position(&to_read);

assert_eq!(answer.as_ref().unwrap().1[0], Position { x:-1, y: 0, z:2});
assert_eq!(answer.as_ref().unwrap().1[1], Position { x:2, y: -10, z:-7});
assert_eq!(answer.as_ref().unwrap().1[2], Position { x:4, y: -8, z:8});
assert_eq!(answer.as_ref().unwrap().1[3], Position { x:3, y: 5, z:-1});
}

#[test]
fn test_one () {    
    let raw_file = 
"<x=-1, y=0, z=2>
<x=2, y=-10, z=-7>
<x=4, y=-8, z=8>
<x=3, y=5, z=-1>
";
    let positions = parse_four_position(&raw_file).unwrap().1;
    let mut states : Vec<State> = Vec::new();
    for i in 0..4  { states.push(State { pos: positions[i] , vel: Velocity { vx:0, vy:0, vz:0 } }); }
    for _i in 0..10 { tick(&mut states); }
    let answer_1 : i32 = states.iter().map(|s| total_energy(s)).sum();
    assert_eq!(answer_1,179);
}

#[test]
fn test_two () {    
    let raw_file = 
"<x=-8, y=-10, z=0>
<x=5, y=5, z=10>
<x=2, y=-7, z=3>
<x=9, y=-8, z=-3>
";
    let positions = parse_four_position(&raw_file).unwrap().1;
    let mut states : Vec<State> = Vec::new();
    for i in 0..4  { states.push(State { pos: positions[i] , vel: Velocity { vx:0, vy:0, vz:0 } }); }
    for _i in 0..100 { tick(&mut states); }
    let answer_1 : i32 = states.iter().map(|s| total_energy(s)).sum();
    assert_eq!(answer_1,1940);
}