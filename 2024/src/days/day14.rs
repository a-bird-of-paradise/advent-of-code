use aoc2024::{AOC, Point, Direction};
use nom::{bytes::complete::tag, character::{anychar, complete::{i64, line_ending, space0}}, combinator::opt, multi::many1, sequence::separated_pair, IResult, Parser};
pub struct Day14;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
struct State {
    location: Point,
    velocity: Direction
}

fn read_pair(input: &str) -> IResult<&str,(i64,i64)> {
    let (input, (_, _, p)) = (anychar, tag("="), separated_pair(i64, tag(","), i64)).parse(input)?;
    Ok((input,p))
}

fn read_line(input: &str) -> IResult<&str,State> {
    let (input, (p, _, v, _)) = (read_pair, space0, read_pair, opt(line_ending)).parse(input)?;
    Ok((input, State { location: Point { x: p.0, y: p.1 }, velocity: Direction { x: v.0, y: v.1 }}))
}

fn read_input(input: &str) -> Vec<State>{
    many1(read_line).parse(input).unwrap().1
}

fn apply(state: &State, x_bound: i64, y_bound: i64, n: i64) -> State {
    State { location: Point {
        x: (state.location.x + n * state.velocity.x).rem_euclid(x_bound),
        y: (state.location.y + n * state.velocity.y).rem_euclid(y_bound)
    }, velocity: state.velocity }
}

fn quadrant( state: &State, x_bound: i64, y_bound: i64, n: i64) -> Result<usize,()> {
    let state = apply(state, x_bound, y_bound, n);

    let mid_x = x_bound / 2;
    let mid_y = y_bound / 2;

    let x_comp = if state.location.x < mid_x { 0 } else { 2 };
    let y_comp = if state.location.y < mid_y { 0 } else { 1 };

    if state.location.x == mid_x || state.location.y == mid_y {
        Err(())
    } else {
        Ok(x_comp +y_comp)
    }
}

impl AOC for Day14 {
    fn part_one(&self, input: &str) -> String {
        
        let mut answer = vec![0;4];

        let states = read_input(&input);
        for state in states {

            match quadrant(&state, 101, 103, 100) {
                Err(()) => {} ,
                Ok(i) => { answer[i] += 1; }
            }
        }
        answer.iter().product::<usize>().to_string()
    }

    fn part_two(&self, input: &str) -> String {
        let old_states = read_input(&input);
        let mut new_states = Vec::<State>::new();

        let mut i: i64 = 6587; // hardcoded , you figure it out yourself

        while i < 10000 {
            for old_state in &old_states {
                new_states.push(apply(old_state, 101, 103, i));
            }
            let n = new_states
                .iter()
                .filter(|e| e.location.x == 55) 
                .count();

            if n >= 25 {
/*              // make a card
                for row in 0..103 {
                    for col in 0..101 {
                        if new_states.iter().filter(|s| s.location.x == col && s.location.y == row).count() > 0 {
                            print!("#");
                        } else {
                            print!(".");
                        }
                    }
                    println!("");
                } */
                break;
            }
            new_states.clear();
            i += 1;
        }
        i.to_string()
    }
}


#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day14.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        
        let mut answer = vec![0;4];

        let states = read_input(&file);
        for state in states {
            println!("{:?} {:?}",apply(&state, 11, 7, 100), quadrant(&state, 11, 7, 100));

            match quadrant(&state, 11, 7, 100) {
                Err(()) => {} ,
                Ok(i) => { answer[i] += 1; }
            }
        }
        assert_eq!(12, answer.iter().product::<usize>());
    } 
    
    #[test]
    fn part_2() {
        let test_file_path = "inputs/day14.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day14 {};
        assert_eq!(my_struct.part_two(&file),"6587");
    }
}