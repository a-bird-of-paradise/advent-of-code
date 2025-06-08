
use aoc2024::AOC;
use nom::{bytes::complete::tag, character::complete::{i64, line_ending}, combinator::opt, multi::many1, IResult, Parser};
pub struct Day13;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct Problem {
    a: i64,
    b: i64,
    c: i64,
    d: i64,
    x: i64,
    y: i64
}

impl Problem {

    fn mutate_for_part_2(&self) -> Self {
        Self { a: self.a, b: self.b, c: self.c, d: self.d, x: self.x + 10000000000000, y: self.y + 10000000000000 }
    }

    fn solve(&self) -> Result<(i64,i64),()> {
        /*
        Matrix problem: note non standard labelling! 
        /     \ /   \   /   \
        | a c | | A |   | x |
        |     | |   | = |   |
        | b d | | B |   | y |
        \     / \   /   \   /
        
        Mp = q. 
        p = M^-1 q. 

        /   \         /       \ /   \
        | A |     1   |  d -c | | x |
        |   | = ----- |       | |   |
        | B |   ad-bc | -b  a | | y |
        \   /         \       / \   /

        so: 
        ad != bc
        dx-cy and ay-bx exactly divisible by ad-bc 
        result of both divisions are > 0 

         */
        let discrim = self.a * self.d - self.b * self.c;
        let top = self.d * self.x - self.c * self.y;
        let bottom = self.a*self.y - self.b*self.x;
        let remainder_top = top % discrim;
        let remainder_bottom = bottom % discrim;

        let no_solution = (self.a * self.d == self.b * self.c) 
            || remainder_bottom != 0 
            || remainder_top != 0 
            || top / discrim <= 0 
            || bottom / discrim <= 0;

        if no_solution {
            Err(())
        } else {
            Ok((top / discrim, bottom / discrim))
        }

    }
}

fn read_a(input: &str) -> IResult<&str,(i64,i64)> {
    let (input,( _, first, _, second, _)) = (tag("Button A: X+"),i64,tag(", Y+"),i64,opt(line_ending)).parse(input)?;
    Ok((input, (first,second)))
}

fn read_b(input: &str) -> IResult<&str,(i64,i64)> {
    let (input,( _, first, _, second, _)) = (tag("Button B: X+"),i64,tag(", Y+"),i64,opt(line_ending)).parse(input)?;
    Ok((input, (first,second)))
}

fn read_ans(input: &str) -> IResult<&str,(i64,i64)> {
    let (input,( _, first, _, second, _)) = (tag("Prize: X="),i64,tag(", Y="),i64,opt(line_ending)).parse(input)?;
    Ok((input, (first,second)))
}

fn read_problem(input: &str) -> IResult<&str,Problem> {
    let (input, (ax_ay, bx_by, x_y, _)) = (read_a,read_b,read_ans,opt(line_ending)).parse(input)?;
    Ok((input, Problem { a: ax_ay.0, b: ax_ay.1, c: bx_by.0, d: bx_by.1, x: x_y.0, y: x_y.1}))
}

fn read_input(input: &str) -> Vec<Problem> {
    many1(read_problem).parse(input).unwrap().1
}

impl AOC for Day13 {
    fn part_one(&self, input: &str) -> String {
        
        let mut answer = 0;

        for problem in read_input(input) {
            answer += match problem.solve() {
                Err(()) => { 0 },
                Ok((x,y)) => { 3*x + y }
            }
        }

        answer.to_string()
    }

    fn part_two(&self, input: &str) -> String {
        
        let mut answer = 0;

        for problem in read_input(input) {
            answer += match problem.mutate_for_part_2().solve() {
                Err(()) => { 0 },
                Ok((x,y)) => { 3*x + y }
            }
        }

        answer.to_string()
    }
}


#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn part_1() {
        let test_file_path = "test-inputs/day13.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day13 {};
        assert_eq!(my_struct.part_one(&file),"480")
    } 
    
    #[test]
    fn part_2() {
        let test_file_path = "test-inputs/day13.txt";
        let file = fs::read_to_string(&test_file_path).expect("Cannot read file");
        let my_struct = Day13 {};
        assert_eq!(my_struct.part_two(&file),"875318608908");
    }
}